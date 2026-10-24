#!/usr/bin/env python3
"""Research Red: project rule checks. Run before committing:
    python3 tools/research_red/rr_check.py

Checks Research Red text (data/scripts/research_red/*.inc):
  - every text line is at most 34 characters ({PLAYER} counts as 7,
    {STR_VAR_n} as 10, other {CODES} as 1)
  - no ALL-CAPS story names (ELM, IVY, OAK, ...)
Generated files are current (encounters, banter, player docs, TM text, the
Art Kit's overworld sprites). And the registry:
  - every FLAG_RR_* / VAR_RR_* defined in include/constants is listed in docs/FLAGS_VARS.md
Exit code 1 if anything fails.
"""
import glob
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
MAX = 34
CAPS = re.compile(r"\b(ELM|IVY|JENNY|OAK|DAISY|DELIA|MIMEY|BROCK|MUFFIN|GERALD|KUKUI|BURNETT)\b")
WIDTHS = {"PLAYER": 7, "RIVAL": 7, "STR_VAR_1": 10, "STR_VAR_2": 10, "STR_VAR_3": 10}


def visible_len(s):
    n = 0
    for m in re.finditer(r"\{([^}]*)\}|.", s):
        n += WIDTHS.get(m.group(1), 1) if m.group(1) is not None else 1
    return n


def check_text(path, problems):
    label, text, start = None, "", 0
    lines = open(path).read().split("\n")

    def flush():
        if label is None or not text:
            return
        for part in re.split(r"\\[nlp]", text.replace("$", "")):
            if visible_len(part) > MAX:
                problems.append(f"{path}:{start}: {label}: line over {MAX} chars: \"{part}\"")
            if CAPS.search(part):
                problems.append(f"{path}:{start}: {label}: ALL-CAPS name: {CAPS.search(part).group(0)}")

    for i, line in enumerate(lines, 1):
        m = re.match(r"^([A-Za-z0-9_]+)::?\s*$", line.strip())
        if m:
            flush()
            label, text, start = m.group(1), "", i
            continue
        m = re.match(r'^\s*\.string\s+"(.*)"\s*$', line)
        if m:
            text += m.group(1)
        elif line.strip() and not line.strip().startswith("@"):
            flush()
            label, text = None, ""
    flush()


def check_registry(problems):
    defined = set()
    for h in ("include/constants/flags.h", "include/constants/vars.h"):
        defined |= set(re.findall(r"#define ((?:FLAG|VAR)_RR_[A-Z0-9_]+)", open(os.path.join(REPO, h)).read()))
    registry = open(os.path.join(REPO, "docs/FLAGS_VARS.md")).read()
    for name in sorted(defined):
        if name not in registry:
            problems.append(f"docs/FLAGS_VARS.md: {name} is defined but not registered")


def check_generated(problems):
    """Data files whose game version is generated: the generated copy must be current."""
    import subprocess
    for tool in ("build_encounters.py", "build_dex_banter.py", "export_player_docs.py", "proper_case.py",
                 "build_tm_descriptions.py"):
        r = subprocess.run([sys.executable, os.path.join(REPO, "tools/research_red", tool), "--check"],
                           capture_output=True, text=True)
        if r.returncode != 0:
            problems.append((r.stderr or r.stdout).strip())
    # Art Kit: overworld sprites from graphics/research_red/overworld/recipes.toml
    r = subprocess.run([sys.executable, os.path.join(REPO, "tools/research_red/art/ow_maker.py"), "--check"],
                       capture_output=True, text=True)
    if r.returncode != 0:
        problems.append((r.stderr or r.stdout).strip().splitlines()[-1])


def main():
    problems = []
    for path in sorted(glob.glob(os.path.join(REPO, "data/scripts/research_red/*.inc"))):
        check_text(os.path.relpath(path, REPO), problems)
    check_registry(problems)
    check_generated(problems)
    for p in problems:
        print(p)
    print(f"rr_check: {len(problems)} problem(s)")
    sys.exit(1 if problems else 0)


if __name__ == "__main__":
    main()
