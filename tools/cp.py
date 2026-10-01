#!/usr/bin/env python3
"""Competitive-programming helper for this folder, built around the CPH extension.
VS Code calls it from .vscode/tasks.json; see docs/GUIDE.pdf for the full workflow.

    python tools/cp.py test FILE            run FILE on its CPH test cases with diagnostics (F5)
    python tools/cp.py run FILE             compile and run with input typed in the terminal (Ctrl+F5)
    python tools/cp.py stress FILE [-n N]   compare FILE against stress/brute.cpp on stress/gen.cpp tests
    python tools/cp.py new NAME [--usaco]   create NAME.cpp from template/template.cpp and open it
    python tools/cp.py archive [--days D]   move solutions untouched for D days into archive/YYYY-MM/
    python tools/cp.py setup [--check]      rebuild the precompiled header, snippets, shortcuts,
                                            CPH settings, CHEATSHEET.md and the guide's generated parts
    python tools/cp.py docs [--only NAME]   compile docs/GUIDE.pdf and docs/ALGORITHMS.pdf (LaTeX)

Library templates live in library/<category>/*.cpp. Each has @name/@prefix/@key/@desc (and optional
@usage/@complexity) header comments; the text between `// @begin` and `// @end` is what gets pasted.
After adding or editing one, run the "CP: Setup" task to regenerate snippets, shortcuts and docs.
"""
import argparse
import ctypes
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import textwrap
import time
from concurrent.futures import ThreadPoolExecutor
from ctypes import wintypes
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
INCLUDE = ROOT / "include"
LIBRARY = ROOT / "library"
TEMPLATE = ROOT / "template" / "template.cpp"
STRESS = ROOT / "stress"
BUILD = ROOT / ".build"
ARCHIVE = ROOT / "archive"
DOCS = ROOT / "docs"  # LaTeX sources; PDFs land in docs/GUIDE.pdf and docs/ALGORITHMS.pdf
PCH_DIR = INCLUDE / "bits"
PROFILE_NAME = "competitive"  # VS Code profile that gets the snippets, shortcuts and CPH settings

# True: signed overflow, out-of-bounds C-array index and bad shifts crash (exit 0xC000001D) instead of
# silently giving a wrong answer. Costs ~0.35 s on every compile, so it's off by default.
UB_TRAP = False

# One set of flags for CPH, F5 and the stress tester, so a single precompiled header serves all three.
# After changing anything here, run the "CP: Setup" task: it rebuilds the precompiled header AND
# rewrites CPH's flags. Never edit CPH's flags by hand, since a precompiled header built with other flags
# can make GCC crash instead of just being ignored.
COMPILE_FLAGS = [
    "-std=c++17", "-O2", "-DLOCAL",
    "-Wall", "-Wextra", "-Wshadow",
    "-D_GLIBCXX_DEBUG",  # checked STL: out-of-range [] on vectors/strings, bad comparators, ...
    *(["-fsanitize=undefined", "-fsanitize-undefined-trap-on-error"] if UB_TRAP else []),
    f"-I{INCLUDE.as_posix()}",  # debug.h + precompiled bits/stdc++.h
]
LINK_FLAGS = [
    "-static",  # no libstdc++ DLL lookup, so other MinGW installs on PATH can't break the binary
    "-Wl,--stack,268435456",  # 256 MB stack like Codeforces: deep DFS doesn't crash locally
]
FLAGS = COMPILE_FLAGS + LINK_FLAGS

# CPH settings written by `setup` (CPH compiles as: g++ src -o src.bin <Args> -D DEBUG -D CPH).
CPH_SETTINGS = {
    "cph.language.cpp.Args": " ".join(FLAGS),  # CPH splits this on spaces, so no path may contain one
    "cph.general.defaultLanguageTemplateFileLocation": str(TEMPLATE),
    "cph.general.doTemplateFileVariableReplacement": True,  # fills $name$, $input$, ... in the template
    "cph.general.ignoreSTDERROR": True,  # otherwise any dbg() output makes CPH fail the test
}

CRASH_CODES = {
    0xC00000FD: "stack overflow (recursion too deep or a huge local array)",
    0xC0000005: "access violation (out-of-bounds pointer or array access)",
    0xC000001D: "undefined behavior trapped (signed overflow, out-of-bounds index or bad shift)",
    0xC0000094: "integer division by zero",
    0xC0000409: "abort / failed runtime check",
    0xC0000374: "heap corruption (writing past the end of an array?)",
    3: "abort(): failed assert or _GLIBCXX_DEBUG check (message in stderr)",
}


def say(msg=""):
    print(f"[cp] {msg}" if msg else "", flush=True)


def die(msg):
    say(msg)
    sys.exit(1)


def gxx():
    path = shutil.which("g++")
    if not path:
        die("g++ not found on PATH")
    return path


# ---------------------------------------------------------------- CPH problem files

def cph_path(path):
    """A path spelled the way VS Code hands it to CPH (lower-case drive letter), which CPH hashes."""
    s = str(path.resolve())
    return s[0].lower() + s[1:] if s[1:2] == ":" else s


def find_cph_problem(src):
    """CPH stores a file's test cases in .cph/.<file>_<md5 of its path>.prob next to it."""
    cph_dir = src.parent / ".cph"
    exact = cph_dir / f".{src.name}_{hashlib.md5(cph_path(src).encode()).hexdigest()}.prob"
    if exact.exists():
        return exact
    if cph_dir.is_dir():
        for prob in cph_dir.iterdir():
            if prob.name.startswith(f".{src.name}_") and prob.name.endswith(".prob"):
                return prob
    return None


def load_cph_tests(src):
    prob = find_cph_problem(src)
    if not prob:
        return None, []
    data = json.loads(prob.read_text(encoding="utf-8"))
    return data, [t for t in data.get("tests", []) if t.get("input", "").strip()]


def render_template(problem):
    """Fill template.cpp exactly like CPH's doTemplateFileVariableReplacement: the first `$key$` of each
    field becomes JSON.stringify(value) minus its first and last character."""
    text = TEMPLATE.read_text(encoding="utf-8")
    now = time.localtime()
    fields = dict(problem, date=time.strftime("%Y-%m-%d", now), time=time.strftime("%H:%M:%S", now))
    for key, value in fields.items():
        text = text.replace(f"${key}$", json.dumps(value, ensure_ascii=False, separators=(",", ":"))[1:-1], 1)
    return text


# ---------------------------------------------------------------- compiling

def pch_stamp():
    version = subprocess.run([gxx(), "--version"], capture_output=True, text=True).stdout.splitlines()[0]
    return f"{gxx()}\n{version}\n{' '.join(COMPILE_FLAGS)}\n"


def ensure_pch(force=False):
    """Precompile bits/stdc++.h with exactly COMPILE_FLAGS (saves ~0.5 s warm, ~3 s cold, per compile).

    Rebuilt whenever the compiler or COMPILE_FLAGS change (tracked in pch.stamp), because GCC does not
    reliably reject a .gch built with different flags. CPH's extra -D DEBUG -D CPH don't invalidate it.
    """
    header, gch, stamp = PCH_DIR / "stdc++.h", PCH_DIR / "stdc++.h.gch", PCH_DIR / "pch.stamp"
    want = pch_stamp()
    if not force and gch.exists() and stamp.exists() and stamp.read_text() == want:
        return
    # Locate the compiler's own bits/stdc++.h (-H prints each header it opens).
    probe = subprocess.run([gxx(), "-H", "-fsyntax-only", "-x", "c++", "-"],
                           input="#include <bits/stdc++.h>\n", capture_output=True, text=True)
    first = next((line for line in probe.stderr.splitlines() if line.startswith(". ")), None)
    if not first:
        die("could not locate bits/stdc++.h:\n" + probe.stderr)
    PCH_DIR.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(first[2:].strip(), header)
    say("building precompiled bits/stdc++.h (one time, ~5 s) ...")
    tmp = gch.with_name("stdc++.h.gch.tmp")
    r = subprocess.run([gxx(), *COMPILE_FLAGS, "-x", "c++-header", str(header), "-o", str(tmp)])
    if r.returncode != 0:
        die("precompiled header build failed")
    os.replace(tmp, gch)  # atomic, so a CPH compile running right now never sees half a file
    stamp.write_text(want)


def exe_path(src, tag=""):
    digest = hashlib.md5(str(src.resolve()).lower().encode()).hexdigest()[:6]
    return BUILD / f"{tag}{src.stem}-{digest}.exe"


def compile_cpp(src, tag=""):
    """Compile src into .build/ unless the existing binary is newer than everything it depends on."""
    ensure_pch()
    exe = exe_path(src, tag)
    deps = [src, INCLUDE / "debug.h", PCH_DIR / "pch.stamp"]
    if exe.exists() and exe.stat().st_mtime > max(d.stat().st_mtime for d in deps if d.exists()):
        return exe
    BUILD.mkdir(exist_ok=True)
    start = time.perf_counter()
    r = subprocess.run([gxx(), str(src), "-o", str(exe), *FLAGS])
    if r.returncode != 0:
        die(f"compilation of {src.name} failed")
    say(f"compiled {src.name} in {time.perf_counter() - start:.1f} s")
    return exe


# ---------------------------------------------------------------- running

class PROCESS_MEMORY_COUNTERS(ctypes.Structure):
    _fields_ = [("cb", wintypes.DWORD), ("PageFaultCount", wintypes.DWORD)] + [
        (name, ctypes.c_size_t) for name in (
            "PeakWorkingSetSize", "WorkingSetSize", "QuotaPeakPagedPoolUsage", "QuotaPagedPoolUsage",
            "QuotaPeakNonPagedPoolUsage", "QuotaNonPagedPoolUsage", "PagefileUsage", "PeakPagefileUsage")]


def quiet_crashes():
    """No 'program has stopped working' dialogs: a crashing child just exits (children inherit this)."""
    if os.name == "nt":
        ctypes.windll.kernel32.SetErrorMode(0x0001 | 0x0002 | 0x8000)


def describe_exit(code):
    code &= 0xFFFFFFFF
    if code == 0:
        return "exit 0"
    meaning = CRASH_CODES.get(code)
    shown = f"0x{code:08X}" if code > 0xFFFF else str(code)
    return f"exit {shown}" + (f": {meaning}" if meaning else "")


def process_stats(handle):
    """(cpu ms, peak memory MB) of a finished process; the open handle keeps its accounting alive."""
    times = [wintypes.FILETIME() for _ in range(4)]
    ctypes.windll.kernel32.GetProcessTimes(handle, *map(ctypes.byref, times))
    cpu_ms = sum((t.dwHighDateTime << 32 | t.dwLowDateTime) for t in times[2:]) / 10_000
    pmc = PROCESS_MEMORY_COUNTERS(cb=ctypes.sizeof(PROCESS_MEMORY_COUNTERS))
    ctypes.windll.psapi.GetProcessMemoryInfo(handle, ctypes.byref(pmc), pmc.cb)
    ctypes.windll.kernel32.CloseHandle(handle)
    return cpu_ms, pmc.PeakPagefileUsage / 2**20


def open_handle(pid):
    return ctypes.windll.kernel32.OpenProcess(0x0400 | 0x0010, False, pid)  # query info + vm read


def cmd_run(args):
    src = Path(args.file).resolve()
    if src.suffix != ".cpp":
        die(f"{src.name} is not a .cpp file")
    exe = compile_cpp(src)
    quiet_crashes()
    say(f"running {src.name}  (type the input; Ctrl+Z then Enter sends EOF)")
    start = time.perf_counter()
    proc = subprocess.Popen([str(exe)])
    handle = open_handle(proc.pid)
    code = proc.wait()
    wall = time.perf_counter() - start
    stats = ""
    if handle:
        cpu_ms, mem = process_stats(handle)
        stats = f" | cpu {cpu_ms:.0f} ms | peak memory {mem:.1f} MB"
    print(flush=True)
    say(f"{describe_exit(code)}{stats} | wall {wall:.2f} s")
    sys.exit(0 if code == 0 else 1)


def run_test(exe, stdin, timeout):
    """Run once with the given input: (exit code or None on timeout, stdout, stderr, wall ms, peak MB)."""
    start = time.perf_counter()
    proc = subprocess.Popen([str(exe)], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                            text=True)
    handle = open_handle(proc.pid)
    try:
        out, err = proc.communicate(stdin, timeout=timeout)
        code = proc.returncode & 0xFFFFFFFF
    except subprocess.TimeoutExpired:
        proc.kill()
        out, err = proc.communicate()
        code = None
    wall_ms = (time.perf_counter() - start) * 1000
    mem = process_stats(handle)[1] if handle else 0.0
    return code, out, err, wall_ms, mem


def show(title, text, limit=40):
    lines = text.rstrip("\n").splitlines()
    print(f"---- {title} ----")
    print("\n".join(lines[:limit]) + (f"\n... ({len(lines) - limit} more lines)" if len(lines) > limit else ""))


def cmd_test(args):
    """F5: run the current file on its CPH test cases, with the details CPH's panel doesn't show."""
    src = Path(args.file).resolve()
    if src.suffix != ".cpp":
        die(f"{src.name} is not a .cpp file")
    problem, tests = load_cph_tests(src)
    if not tests:
        say("no CPH test cases for this file yet, so running with typed input instead")
        return cmd_run(args)
    exe = compile_cpp(src)
    quiet_crashes()
    limit = problem.get("timeLimit") or 0
    passed = 0
    say(f"{problem.get('name', src.stem)}: {len(tests)} CPH test case(s)"
        + (f", time limit {limit} ms" if limit else ""))
    for i, t in enumerate(tests, 1):
        code, out, err, wall_ms, mem = run_test(exe, t["input"], timeout=max(5.0, 3 * limit / 1000))
        expected = t.get("output", "")
        if code is None:
            verdict = "TIME LIMIT (killed)"
        elif code != 0:
            verdict = f"RUNTIME ERROR, {describe_exit(code)}"
        elif out.split() != expected.split():
            verdict = "WRONG ANSWER"
        else:
            verdict = "PASS"
        slow = f"  (over the {limit} ms limit; the debug STL is slower than the judge)" if limit and wall_ms > limit else ""
        say(f"test {i}: {verdict}  {wall_ms:.0f} ms, {mem:.1f} MB{slow}")
        if verdict == "PASS":
            passed += 1
            continue
        show("input", t["input"])
        show("expected", expected)
        show("got", out)
        if err.strip():
            show("stderr", err, limit=25)
    say(f"{passed}/{len(tests)} passed")
    sys.exit(0 if passed == len(tests) else 1)


# ---------------------------------------------------------------- stress testing

def run_capture(exe, argv=(), stdin="", timeout=5.0):
    try:
        r = subprocess.run([str(exe), *argv], input=stdin, capture_output=True, text=True, timeout=timeout)
        return r.returncode & 0xFFFFFFFF, r.stdout, r.stderr
    except subprocess.TimeoutExpired:
        return None, "", ""


def copy_to_clipboard(text):
    try:
        subprocess.run(["clip"], input=text, text=True, check=True)  # text mode already writes \r\n on Windows
        return True
    except (OSError, subprocess.CalledProcessError):
        return False


def cmd_stress(args):
    src = Path(args.file).resolve()
    gen, brute, checker = STRESS / "gen.cpp", STRESS / "brute.cpp", STRESS / "checker.cpp"
    if src.parent == STRESS.resolve():
        die("open your solution file (not gen/brute/checker) and run the stress test from there")
    for f in (gen, brute):
        if not f.exists():
            die(f"missing {f.relative_to(ROOT)}")
    exes = {"sol": compile_cpp(src), "gen": compile_cpp(gen, "stress_"), "brute": compile_cpp(brute, "stress_")}
    if checker.exists():
        exes["checker"] = compile_cpp(checker, "stress_")
    quiet_crashes()

    _, samples = load_cph_tests(src)  # a brute force that fails the samples would make every verdict wrong
    for i, t in enumerate(samples, 1):
        code, got, _ = run_capture(exes["brute"], stdin=t["input"], timeout=args.timeout)
        if code != 0 or got.split() != t.get("output", "").split():
            show("sample input", t["input"])
            show("expected", t.get("output", ""))
            show("brute.cpp printed", got)
            die(f"brute.cpp fails CPH sample {i}: fix the brute force first")
    if samples:
        say(f"brute.cpp passes all {len(samples)} CPH sample(s)")

    say(f"stress testing {src.name} against brute.cpp ({args.n} tests"
        f"{', verdicts by checker.cpp' if checker.exists() else ''})")
    for i in range(1, args.n + 1):
        code, test, err = run_capture(exes["gen"], [str(i)])
        if code != 0:
            die(f"gen.cpp failed on seed {i} ({'timeout' if code is None else describe_exit(code)})\n{err}")
        exp_code, expected, _ = run_capture(exes["brute"], stdin=test, timeout=args.timeout)
        if exp_code != 0:
            show("input", test)
            die(f"brute.cpp failed on seed {i} ({'timeout' if exp_code is None else describe_exit(exp_code)})")
        code, got, err = run_capture(exes["sol"], stdin=test, timeout=args.timeout)
        problem = None
        if code is None:
            problem = f"time limit ({args.timeout:g} s)"
        elif code != 0:
            problem = describe_exit(code)
        elif "checker" in exes:
            files = [BUILD / f"stress_{name}.txt" for name in ("in", "expected", "got")]
            for path, text in zip(files, (test, expected, got)):
                path.write_text(text)
            c_code, c_out, c_err = run_capture(exes["checker"], [str(p) for p in files])
            if c_code != 0:
                problem = f"checker rejected: {(c_out + c_err).strip()}"
        elif expected.split() != got.split():
            problem = "wrong answer"
        if problem:
            print(flush=True)
            say(f"FAILED on test {i}: {problem}")
            show("input", test)
            show("expected (brute)", expected)
            show("got (yours)", got)
            if err.strip():
                show("your stderr", err, limit=20)
            (STRESS / "fail.in").write_text(test)
            where = "copied to the clipboard" if copy_to_clipboard(test) else "saved to stress/fail.in"
            say(f"input {where}: in CPH press '+ New Testcase', paste it, and use the expected output above")
            sys.exit(1)
        if i % 20 == 0 or i == args.n:
            print(f"\r[cp] {i}/{args.n} passed", end="", flush=True)
    print(flush=True)
    say(f"all {args.n} tests passed")


# ---------------------------------------------------------------- new / archive

def open_in_vscode(path):
    code = shutil.which("code")
    if code:
        subprocess.run([code, "-r", str(path)])
    else:
        say(f"open {path} (VS Code 'code' command not on PATH)")


def cmd_new(args):
    stem = re.sub(r"[^\w\-]+", "_", re.sub(r"\.cpp$", "", args.name.strip())).strip("_") or "sol"
    path = ROOT / f"{stem}.cpp"
    if path.exists():
        say(f"{path.name} already exists; opening it")
    else:
        io = ({"type": "file", "fileName": f"{stem}.in"}, {"type": "file", "fileName": f"{stem}.out"}) \
            if args.usaco else ({"type": "stdin"}, {"type": "stdout"})
        problem = {"name": args.name.strip(), "group": "USACO" if args.usaco else "local", "url": "",
                   "input": io[0], "output": io[1]}
        path.write_text(render_template(problem), encoding="utf-8", newline="\n")
        say(f"created {path.name}" + (f"  (reads {stem}.in / writes {stem}.out on the judge)" if args.usaco else ""))
    open_in_vscode(path)


def move_cph_tests(src, dest):
    """Re-key the CPH test file of src (named after the md5 of its path) for its new path dest."""
    cph_dir = src.parent / ".cph"
    if not cph_dir.is_dir():
        return
    for prob in cph_dir.iterdir():
        if not (prob.name.startswith(f".{src.name}_") and prob.name.endswith(".prob")):
            continue
        data = json.loads(prob.read_text(encoding="utf-8"))
        data["srcPath"] = cph_path(dest)
        new_dir = dest.parent / ".cph"
        new_dir.mkdir(exist_ok=True)
        new_name = f".{dest.name}_{hashlib.md5(data['srcPath'].encode()).hexdigest()}.prob"
        (new_dir / new_name).write_text(json.dumps(data), encoding="utf-8")
        prob.unlink()


def cmd_archive(args):
    cutoff = time.time() - args.days * 86400
    moved = []
    for src in sorted(ROOT.glob("*.cpp")):
        mtime = src.stat().st_mtime
        if mtime > cutoff:
            continue
        dest_dir = ARCHIVE / time.strftime("%Y-%m", time.localtime(mtime))
        dest_dir.mkdir(parents=True, exist_ok=True)
        dest, n = dest_dir / src.name, 2
        while dest.exists():
            dest, n = dest_dir / f"{src.stem}_{n}.cpp", n + 1
        shutil.move(str(src), str(dest))
        move_cph_tests(src, dest)
        moved.append(dest.relative_to(ROOT))
    junk = [p for pattern in ("*.exe", "*.bin") for p in ROOT.glob(pattern)]
    for p in junk:
        p.unlink(missing_ok=True)
    for m in moved:
        say(f"archived {m}")
    say(f"{len(moved)} solution(s) archived (untouched for {args.days:g}+ days, CPH tests kept), "
        f"{len(junk)} binary file(s) deleted")


# ---------------------------------------------------------------- setup: snippets, shortcuts, CPH, docs

SNIPPET_HEADER = re.compile(r"\s*//\s*@(name|prefix|key|desc|usage|complexity)\b ?(.*)")
KB_BEGIN = "// >>> cp-library shortcuts (generated by tools/cp.py setup; edits inside this block get overwritten)"
KB_END = "// <<< cp-library shortcuts"
# t: core (Bronze-Gold), y: advanced (Platinum+), u: specialist techniques (second edition of the handbook)
LEADERS = {"t": "ctrl+alt+t", "y": "ctrl+alt+y", "u": "ctrl+alt+u"}
CATEGORIES = {
    "template": "Template", "basics": "Basics: input, search, sorting, prefix sums, two pointers",
    "graph": "Graphs", "tree": "Trees", "ds": "Data structures & range queries", "dp": "Dynamic programming",
    "math": "Math", "string": "Strings", "geometry": "Geometry", "flow": "Flows & matching", "misc": "Misc",
}
# How the full-template snippet fills the placeholders CPH normally fills (VS Code snippet variables).
TEMPLATE_SNIPPET_VARS = {
    "name": "${TM_FILENAME_BASE}", "group": "", "url": "",
    "date": "${CURRENT_YEAR}-${CURRENT_MONTH}-${CURRENT_DATE}",
    "time": "${CURRENT_HOUR}:${CURRENT_MINUTE}:${CURRENT_SECOND}",
    "input": '"type":"stdin"', "output": '"type":"stdout"',
}
TEX_SPECIAL = {"\\": r"\textbackslash{}", "&": r"\&", "%": r"\%", "$": r"\$", "#": r"\#", "_": r"\_",
               "{": r"\{", "}": r"\}", "~": r"\textasciitilde{}", "^": r"\textasciicircum{}",
               "<": r"\textless{}", ">": r"\textgreater{}", "|": r"\textbar{}", "`": r"\textasciigrave{}",
               "'": r"\textquotesingle{}"}
KEYBOARD_ROWS = [("`1234567890-=", 0.0), ("qwertyuiop[]", 0.5), ("asdfghjkl;'", 0.75), ("zxcvbnm,./", 1.25)]


def parse_key(spec, where):
    parts = spec.split()
    if len(parts) != 2 or parts[0] not in LEADERS:
        die(f"{where}: @key must look like 't d', 'y shift+k' or 'u q', got '{spec}'")
    return parts[0], parts[1]


def load_library():
    template = TEMPLATE.read_text(encoding="utf-8").strip("\n")
    items = [dict(name="Full template", prefix="cptemplate", key=("t", "t"), cat="template",
                  desc="[empty file] the whole template, for files CPH didn't create", body=template,
                  usage=[], complexity="", path=TEMPLATE)]
    for f in sorted(LIBRARY.rglob("*.cpp")):
        meta, usage, body, inside = {}, [], [], False
        for line in f.read_text(encoding="utf-8").splitlines():
            m = SNIPPET_HEADER.match(line)
            if m and not inside and not body:
                if m[1] == "usage":
                    usage.append(m[2].strip())
                else:
                    meta[m[1]] = m[2].strip()
            elif line.strip() == "// @begin":
                inside = True
            elif line.strip() == "// @end":
                inside = False
            elif inside:
                body.append(line)
        missing = {"name", "prefix", "key", "desc"} - meta.keys()
        if missing or not body:
            die(f"{f.relative_to(ROOT)}: missing {', '.join(sorted(missing)) or '// @begin ... // @end body'}")
        if f.parent.name not in CATEGORIES:
            die(f"{f.relative_to(ROOT)}: unknown category folder '{f.parent.name}'")
        items.append(dict(meta, key=parse_key(meta["key"], f.name), cat=f.parent.name, usage=usage,
                          complexity=meta.get("complexity", ""),
                          body=textwrap.dedent("\n".join(body)).strip("\n"), path=f))
    for field in ("key", "prefix", "name"):
        seen = {}
        for it in items:
            if it[field] in seen:
                die(f"duplicate @{field} {it[field]!r}: {seen[it[field]].name} and {it['path'].name}")
            seen[it[field]] = it["path"]
    order = list(CATEGORIES)
    items.sort(key=lambda it: (order.index(it["cat"]), it["name"].lower()))
    return items


def profile_dir():
    user = Path(os.environ["APPDATA"]) / "Code" / "User"
    try:
        storage = json.loads((user / "globalStorage" / "storage.json").read_text(encoding="utf-8"))
        for p in storage.get("userDataProfiles", []):
            if p.get("name") == PROFILE_NAME:
                return user / "profiles" / p["location"]
    except (OSError, ValueError):
        pass
    say(f"VS Code profile '{PROFILE_NAME}' not found; using the default profile")
    return user


def backup_once(path):
    bak = path.with_name(path.name + ".bak-cp-setup")
    if path.exists() and not bak.exists():
        shutil.copyfile(path, bak)


def snippet_title(it):
    return f"CP: {it['name']}"


def pretty_key(key):
    leader, k = key
    shown = "+".join(p.capitalize() if len(p) > 1 else p.upper() for p in k.split("+"))
    return f"Ctrl+Alt+{leader.upper()}, {shown}"


def where_to_paste(it):
    m = re.match(r"\[([^\]]+)\]\s*(.*)", it["desc"])
    return (m[1], m[2]) if m else ("", it["desc"])


def snippet_body(it):
    body = it["body"].replace("\\", "\\\\").replace("$", "\\$")  # snippet syntax escapes
    if it["cat"] == "template":
        for key, value in TEMPLATE_SNIPPET_VARS.items():
            body = body.replace(f"\\${key}\\$", value)
    return body


def write_snippets(items, profile):
    snippets = {}
    for it in items:
        snippets[snippet_title(it)] = {
            "scope": "cpp",
            "prefix": it["prefix"],
            "description": f"{it['desc']}   [{pretty_key(it['key'])}]",
            "body": snippet_body(it).split("\n"),
        }
    out = profile / "snippets" / "cp-library.code-snippets"
    out.parent.mkdir(exist_ok=True)
    out.write_text(json.dumps(snippets, indent=2) + "\n", encoding="utf-8")
    return out


def write_keybindings(items, profile):
    when = "editorTextFocus && !editorReadonly && editorLangId == 'cpp'"
    entries = [{"key": f"{LEADERS[it['key'][0]]} {it['key'][1]}", "command": "editor.action.insertSnippet",
                "args": {"name": snippet_title(it)}, "when": when} for it in items]
    entries += [{"key": f"{leader} space", "command": "editor.action.insertSnippet", "when": when}
                for leader in LEADERS.values()]  # searchable picker of every snippet
    entries += [
        {"key": "ctrl+alt+x", "command": "workbench.action.tasks.runTask", "args": "CP: Stress Test",
         "when": "editorLangId == 'cpp'"},
        {"key": "ctrl+f5", "command": "workbench.action.tasks.runTask", "args": "CP: Run With Typed Input",
         "when": "editorLangId == 'cpp' && !inDebugMode"},
    ]
    block = "\n".join(["  " + KB_BEGIN, *(f"  {json.dumps(e)}," for e in entries), "  " + KB_END])
    path = profile / "keybindings.json"
    text = path.read_text(encoding="utf-8") if path.exists() else "[\n]\n"
    backup_once(path)
    if KB_BEGIN in text:
        start = text.index("  " + KB_BEGIN) if ("  " + KB_BEGIN) in text else text.index(KB_BEGIN)
        end = text.index(KB_END, start) + len(KB_END)
        text = text[:start] + block + text[end:]
    else:
        close = text.rindex("]")
        head = text[:close].rstrip()
        if head.endswith("}"):
            head += ","
        text = head + "\n\n" + block + "\n" + text[close:]
    path.write_text(text, encoding="utf-8")
    return path


def set_json_value(text, key, value):
    """Set one top-level key in a settings.json (JSONC) file, keeping everything else as written."""
    literal = r'"(?:[^"\\]|\\.)*"|true|false|null|-?\d+(?:\.\d+)?'
    pattern = re.compile(r'("%s"\s*:\s*)(?:%s)' % (re.escape(key), literal))
    if pattern.search(text):
        return pattern.sub(lambda m: m[1] + json.dumps(value), text, count=1)
    brace = text.index("{")
    return text[:brace + 1] + f"\n    {json.dumps(key)}: {json.dumps(value)}," + text[brace + 1:]


def write_cph_settings(profile):
    path = profile / "settings.json"
    text = path.read_text(encoding="utf-8") if path.exists() else "{\n}\n"
    backup_once(path)
    for key, value in CPH_SETTINGS.items():
        text = set_json_value(text, key, value)
    path.write_text(text, encoding="utf-8")
    return path


def md_escape(s):
    return s.replace("|", "\\|")


def write_cheatsheet(items):
    lines = [
        "# Competitive programming cheatsheet",
        "",
        "Generated by `python tools/cp.py setup`. Full manual: [docs/GUIDE.pdf](docs/GUIDE.pdf); "
        "algorithms handbook: [docs/ALGORITHMS.pdf](docs/ALGORITHMS.pdf).",
        "",
        "## Shortcuts",
        "",
        "| Keys | Action |",
        "|---|---|",
        "| `Ctrl+Alt+B` | CPH: run all test cases (the main button) |",
        "| `Ctrl+Alt+T` / `Ctrl+Alt+Y` / `Ctrl+Alt+U`, then a key | paste a template at the cursor (core / advanced / "
        "specialist, tables below) |",
        "| `Ctrl+Alt+T`, `Space` | searchable picker of every template |",
        "| prefix + `Tab` | same, e.g. `cpdsu` + Tab |",
        "| `F5` | run the CPH test cases in the terminal with crash reason, time and memory |",
        "| `Ctrl+F5` | run with input typed in the terminal |",
        "| `Ctrl+Alt+X` | stress test against `stress/brute.cpp` with `stress/gen.cpp` |",
        "| `Ctrl+Shift+N` / `Ctrl+Shift+U` | new problem / new USACO problem from the template |",
        "| `Ctrl+Shift+Alt+C` | archive solutions untouched for a day (CPH tests kept) |",
        "",
        "`dbg(x, v, mp)` prints to CPH's Standard Error box locally and compiles to nothing on the judge.",
        "",
        "## Templates",
        "",
        "`global` goes above `solve()`; `inside solve` goes where you need it and uses the variables it lists.",
    ]
    for cat, title in CATEGORIES.items():
        group = [it for it in items if it["cat"] == cat]
        if not group:
            continue
        lines += ["", f"### {title}", "", "| Keys | Prefix | Template | Where | Usage |", "|---|---|---|---|---|"]
        for it in group:
            where, desc = where_to_paste(it)
            lines.append(f"| `{pretty_key(it['key'])}` | `{it['prefix']}` | {it['name']} | {where} | {md_escape(desc)} |")
    out = ROOT / "CHEATSHEET.md"
    out.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return out


def tex(s):
    return "".join(TEX_SPECIAL.get(c, c) for c in s)


def write_docs_generated(items):
    """docs/generated/library_reference.tex (every template) and keymaps.tex (keyboard figures)."""
    out_dir = DOCS / "generated"
    out_dir.mkdir(parents=True, exist_ok=True)
    ref = ["% generated by tools/cp.py setup -- do not edit"]
    for cat, title in CATEGORIES.items():
        group = [it for it in items if it["cat"] == cat]
        if not group:
            continue
        ref.append(f"\\section{{{tex(title)}}}")
        for it in group:
            where, desc = where_to_paste(it)
            where_txt = {"global": "above solve()", "inside solve": "inside solve()"}.get(where, where)
            source = it["path"].relative_to(ROOT).as_posix()
            ref.append(f"\\begin{{libentry}}{{{tex(it['name'])}}}{{{tex(pretty_key(it['key']))}}}{{{tex(it['prefix'])}}}"
                       f"{{{tex(where_txt)}}}{{{tex(it['complexity'] or '--')}}}{{{tex(source)}}}")
            ref.append(tex(desc))
            if it["usage"]:
                ref += ["\\begin{lstlisting}", *it["usage"], "\\end{lstlisting}"]
            ref.append("\\end{libentry}")
    (out_dir / "library_reference.tex").write_text("\n".join(ref) + "\n", encoding="utf-8")

    # the handbook's Appendix C: every template with its keys, prefix and the page that explains it
    idx = ["% generated by tools/cp.py -- do not edit"]
    for cat, title in CATEGORIES.items():
        group = [it for it in items if it["cat"] == cat and cat != "template"]
        if not group:
            continue
        idx += [f"\\section*{{{tex(title)}}}", "\\begin{small}",
                "\\begin{longtable}{@{}p{7.2cm}p{3.5cm}p{3.3cm}r@{}}", "\\toprule",
                "\\thd{Template} & \\thd{Keys} & \\thd{Prefix} & \\thd{Page} \\\\", "\\midrule", "\\endhead"]
        for it in group:
            idx.append(f"{tex(it['name'])} & {tex(pretty_key(it['key']))} & \\texttt{{{tex(it['prefix'])}}} & "
                       f"\\tplpage{{{it['prefix']}}} \\\\")
        idx += ["\\bottomrule", "\\end{longtable}", "\\end{small}"]
    (out_dir / "template_index.tex").write_text("\n".join(idx) + "\n", encoding="utf-8")

    by_key = {it["key"]: it for it in items}
    figs = ["% generated by tools/cp.py setup -- do not edit"]
    for leader in LEADERS:
        leader_name = f"Ctrl+Alt+{leader.upper()}"
        for shift in (False, True):
            if leader == "u" and shift and not any(k[0] == "u" and k[1].startswith("shift+") for k in by_key):
                continue  # the specialist layer only needs its Shift keyboard once templates use it
            used = 0
            nodes = []
            for row, (keys, offset) in enumerate(KEYBOARD_ROWS):
                for col, ch in enumerate(keys):
                    k = f"shift+{ch}" if shift else ch
                    it = by_key.get((leader, k))
                    x, y = (col + offset) * 1.24, -row * 1.12
                    label = f"\\textbf{{\\color{{cpblue}}{tex(ch.upper() if ch.isalpha() else ch)}}}"
                    if it:
                        used += 1
                        name = it["prefix"][2:]
                        name = name if len(name) <= 10 else name[:9] + "."
                        nodes.append(f"\\node[kb, kbused] at ({x:.2f},{y:.2f}) {{{label}\\\\{tex(name)}}};")
                    else:
                        nodes.append(f"\\node[kb] at ({x:.2f},{y:.2f}) {{{label}}};")
            layer = "with Shift" if shift else "no Shift"
            figs += ["\\begin{figure}[H]\\centering",
                     "\\begin{tikzpicture}[kb/.style={draw=cpgray!45, fill=white, rounded corners=2pt, minimum width=11.6mm,"
                     " minimum height=10.4mm, align=center, font=\\sffamily\\fontsize{5.6}{6.4}\\selectfont, inner sep=0.6pt,"
                     " text=cpgray!70}, kbused/.style={fill=cporange!10, draw=cporange!70, text=black}]",
                     *nodes, "\\end{tikzpicture}",
                     f"\\caption{{{leader_name}, then a key {layer} ({used} templates)}}", "\\end{figure}"]
    (out_dir / "keymaps.tex").write_text("\n".join(figs) + "\n", encoding="utf-8")
    return out_dir


def cmd_docs(args):
    """Compile docs/guide.tex and docs/algorithms.tex into docs/GUIDE.pdf and docs/ALGORITHMS.pdf.
    With --chapter NN only that chapter of the handbook is typeset (into docs/build/algorithms.pdf, for a quick look);
    cross-references and page numbers then come from the last full build."""
    pdflatex = shutil.which("pdflatex")
    if not pdflatex:
        die("pdflatex not found (TinyTeX / TeX Live must be on PATH)")
    write_docs_generated(load_library())
    build = DOCS / "build"
    (build / "algorithms").mkdir(parents=True, exist_ok=True)  # \include writes one .aux per chapter there
    targets = [("guide", "GUIDE"), ("algorithms", "ALGORITHMS")]
    if args.only:
        targets = [t for t in targets if t[0] == args.only]
    chapter = None
    if args.chapter:
        matches = sorted(p.stem for p in (DOCS / "algorithms").glob(f"{args.chapter}*.tex"))
        if not matches:
            die(f"no chapter file docs/algorithms/{args.chapter}*.tex")
        chapter, targets = matches[0], [("algorithms", None)]

    def latex(src):
        job = f"{src}.tex" if not chapter else rf"\def\cponly{{algorithms/{chapter}}}\input{{{src}.tex}}"
        try:
            r = subprocess.run([pdflatex, "-interaction=nonstopmode", "-halt-on-error", f"-output-directory={build}",
                                f"-jobname={src}", job], cwd=DOCS, capture_output=True, text=True, encoding="utf-8",
                               errors="replace", timeout=900)
        except subprocess.TimeoutExpired:
            die(f"{src}.tex: pdflatex ran for 15 minutes; a figure is probably looping (see {build / (src + '.log')})")
        if r.returncode != 0:
            errors = [ln for ln in r.stdout.splitlines() if ln.startswith("!") or ln.startswith("l.")]
            die(f"{src}.tex failed:\n" + "\n".join(errors[:12]) + f"\nfull log: {build / (src + '.log')}")

    for src, name in targets:
        start = time.perf_counter()
        for run in range(2 if chapter else 3):  # three passes settle the table of contents and cross-references
            latex(src)
            idx = build / f"{src}.idx"
            if idx.exists() and not chapter and shutil.which("makeindex"):
                rel = build.relative_to(DOCS).as_posix()  # makeindex refuses to write to absolute paths (openout_any=p)
                subprocess.run(["makeindex", "-q", "-s", "cpindex.ist", "-o", f"{rel}/{src}.ind", f"{rel}/{src}.idx"],
                               cwd=DOCS, capture_output=True)
        if chapter:
            say(f"chapter {chapter} -> {build / (src + '.pdf')} in {time.perf_counter() - start:.0f} s")
            continue
        shutil.copyfile(build / f"{src}.pdf", DOCS / f"{name}.pdf")
        say(f"docs/{name}.pdf built in {time.perf_counter() - start:.0f} s")


def check_library(items):
    """Compile every template standalone as strict C++17 (-Wpedantic), both as written and under
    `#define int long long`. __int128 is the one GNU extension allowed (Pollard rho needs it)."""
    BUILD.mkdir(exist_ok=True)
    tmp = Path(tempfile.mkdtemp(dir=BUILD))

    def check(it):
        errors = []
        wrapper = tmp / f"{it['path'].stem}_int64.cpp"
        wrapper.write_text("#include <bits/stdc++.h>\nusing namespace std;\nusing ll = long long;\n"
                           f"#define int long long\n#include \"{it['path'].as_posix()}\"\n")
        for mode, src in (("as written", it["path"]), ("#define int long long", wrapper)):
            if it["cat"] == "template" and mode != "as written":
                continue
            r = subprocess.run([gxx(), *COMPILE_FLAGS, "-Wpedantic", "-fsyntax-only", str(src)],
                               capture_output=True, text=True)
            warnings = [ln for ln in r.stderr.splitlines() if "warning:" in ln and "int128" not in ln]
            if r.returncode != 0 or warnings:
                errors.append(f"{it['path'].relative_to(ROOT)} [{mode}]\n{r.stderr}")
        return errors

    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
        failures = [e for errs in pool.map(check, items) for e in errs]
    shutil.rmtree(tmp, ignore_errors=True)
    for f in failures:
        print(f)
    bad = {f.split(" [")[0] for f in failures}
    say(f"library check: {len(items) - len(bad)}/{len(items)} templates compile cleanly as strict C++17 "
        "in both int modes")
    return not failures


def cmd_setup(args):
    ensure_pch(force=True)
    items = load_library()
    profile = profile_dir()
    say(f"{len(items)} templates -> {write_snippets(items, profile)}")
    say(f"shortcuts -> {write_keybindings(items, profile)}")
    say(f"CPH settings -> {write_cph_settings(profile)}")
    say(f"cheatsheet -> {write_cheatsheet(items)}")
    say(f"guide reference + keyboard maps -> {write_docs_generated(items)} (run 'CP: Build Docs' for the PDFs)")
    if args.check and not check_library(items):
        sys.exit(1)
    say("done. Reload VS Code once (Ctrl+Shift+P > Reload Window) if new shortcuts don't fire yet.")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("test")
    p.add_argument("file")
    p.set_defaults(func=cmd_test)
    p = sub.add_parser("run")
    p.add_argument("file")
    p.set_defaults(func=cmd_run)
    p = sub.add_parser("stress")
    p.add_argument("file")
    p.add_argument("-n", type=int, default=300, help="number of random tests (default 300)")
    p.add_argument("--timeout", type=float, default=5.0, help="seconds per run (default 5)")
    p.set_defaults(func=cmd_stress)
    p = sub.add_parser("new")
    p.add_argument("name")
    p.add_argument("--usaco", action="store_true")
    p.set_defaults(func=cmd_new)
    p = sub.add_parser("archive")
    p.add_argument("--days", type=float, default=1)
    p.set_defaults(func=cmd_archive)
    p = sub.add_parser("setup")
    p.add_argument("--check", action="store_true", help="also compile-check every library template")
    p.set_defaults(func=cmd_setup)
    p = sub.add_parser("docs")
    p.add_argument("--only", choices=["guide", "algorithms"], help="build just one of the two PDFs")
    p.add_argument("--chapter", metavar="NN", help="typeset only this handbook chapter (file prefix, e.g. 13 or 01b)")
    p.set_defaults(func=cmd_docs)
    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
