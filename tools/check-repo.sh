#!/usr/bin/env bash
# Safety check: catches things that must never be saved in this project.
# It runs by itself before every save (git commit), through a hook Claude Hub
# installs. Run it by hand any time: tools/check-repo.sh   (exit 0 = all clear)
#
#   tools/check-repo.sh           checks every file in the project
#   tools/check-repo.sh --staged  checks only what's about to be saved (the hook)
#   tools/check-repo.sh --message FILE  checks a save message (the commit-msg hook)
#
# Finds: secrets (passwords, API keys, login tokens, private keys, .env files),
# personal details (personal email addresses, home-folder paths with a user
# name, the owner's private words), in files and in save messages; in a public
# project, a save signed with anything but the GitHub no-reply address; game files (ROMs), and files too big for GitHub (over 50 MB).
#
# False alarm? Put the file's path on its own line in tools/check-repo.allow
# (ask the owner first). Project-specific rules go at the bottom.
# From the claude-setup project template (hub/template/tools/check-repo.sh).
set -uo pipefail
cd "$(git rev-parse --show-toplevel 2>/dev/null || dirname "$0")" || exit 1
fail=0
# Personal details: personal email addresses and home-folder paths with a user name.
personal='[A-Za-z0-9._%+-]+@(gmail|googlemail|outlook|hotmail|live|yahoo|icloud|me|aol|proton|protonmail)\.[a-z.]+|/home/[a-z_][a-z0-9_-]*(/|$|[^a-z0-9_-])|/mnt/c/Users/[A-Za-z][A-Za-z0-9._-]*|[Cc]:\\+Users\\+[A-Za-z][A-Za-z0-9._-]*'
# The owner's private words (real name, email names, PC user name), kept outside every
# project in this file: one per line, any case, whole words only. Owner, 2026-11-17.
words_file="${CHECK_REPO_WORDS:-$HOME/.config/claude-hub/private-words}"
words=$(grep -v '^#' "$words_file" 2>/dev/null | sed 's/^ *//; s/ *$//; /^$/d')
has_words() { [ -n "$words" ] && grep -qiwF -f <(printf '%s\n' "$words") <<<"$1"; }
problem() { [ $fail = 0 ] && echo "SAFETY CHECK: not saved, please fix first."; fail=1; echo; echo "$1"; sed 's/^/    /' <<<"$2" | head -20; }

if [ "${1:-}" = --message ]; then   # the save message: no personal details there either
    msg=$(grep -v '^#' "${2:-/dev/null}" 2>/dev/null)
    bad=$(grep -nE "$personal" <<<"$msg"; [ -n "$words" ] && grep -niwF -f <(printf '%s\n' "$words") <<<"$msg")
    [ -n "$bad" ] && problem "Personal details in the save message (write ~/ instead of /home/<name>/):" "$(sort -u <<<"$bad")"
    [ $fail = 0 ] || { echo; echo "Change the message, then save again."; }
    exit $fail
fi
# Public project: every save is signed with the GitHub no-reply address, and the name on it
# holds none of the private words (the persona stays the only public name).
if [ "$(git config --get hub.privacy)" = public ]; then
    em=$(git config user.email); nm=$(git config user.name)
    case "$em" in *@users.noreply.github.com) ;; *) problem "Public project: saves must be signed with your GitHub no-reply address (git config user.email), not:" "$em";; esac
    has_words "$nm" && problem "Public project: the name on saves (git config user.name) holds a private word:" "$nm"
fi

if [ "${1:-}" = --staged ]; then
    files=$(git diff --cached --name-only --diff-filter=ACMR)
else
    files=$(git ls-files --cached --others --exclude-standard)
fi
files=$(grep -vxF -e tools/check-repo.sh -e tools/check-repo.allow <<<"$files")
[ -f tools/check-repo.allow ] && files=$(grep -vxFf <(grep -v '^#' tools/check-repo.allow | sed '/^$/d') <<<"$files")
files=$(while IFS= read -r f; do [ -f "$f" ] && printf '%s\n' "$f"; done <<<"$files")
[ -z "$files" ] && { echo "check-repo: all clear (nothing to check)"; exit 0; }

# Secret files by name (the global git ignore normally keeps these out).
bad=$(grep -iE '(^|/)(\.env(\..*)?|.*\.pem|.*\.key|id_rsa.*|id_ed25519.*|.*credentials.*\.json|\.credentials.*|secrets\..*|.*\.secret|.*\.kdbx)$' <<<"$files" | grep -viE '\.env\.example$')
[ -n "$bad" ] && problem "Secret files (keys, passwords, logins):" "$bad"

# Game files: never uploaded or shared.
bad=$(grep -iE '\.(gba|gbc|gb|nds|3ds|nes|sfc|smc|z64|n64|v64|ndd|gcm|wbfs|xci|nsp)$' <<<"$files")
[ -n "$bad" ] && problem "Game files (ROMs):" "$bad"

# Too big for GitHub (it refuses 100 MB; over 50 MB is a warning sign).
bad=$(while IFS= read -r f; do s=$(stat -c %s "$f" 2>/dev/null || echo 0); [ "$s" -gt 52428800 ] && echo "$f ($((s / 1048576)) MB)"; done <<<"$files")
[ -n "$bad" ] && problem "Files over 50 MB (keep them on E: instead):" "$bad"

# Secrets and personal details inside text files.
tokens='BEGIN [A-Z ]*PRIVATE KEY|gh[pousr]_[A-Za-z0-9]{30,}|github_pat_[A-Za-z0-9_]{30,}|sk-ant-[A-Za-z0-9_-]{20,}|sk-[A-Za-z0-9]{32,}|(^|[^A-Za-z0-9+/])AKIA[0-9A-Z]{16}([^A-Za-z0-9+/]|$)|AIza[0-9A-Za-z_-]{35}|xox[abprs]-[A-Za-z0-9-]{10,}'   # exact case
assigned='(api[_-]?key|secret|password|passwd|token)["'"'"']? *[:=] *["'"'"'][^"'"'"' ]{8,}'   # any case
bad=$( { tr '\n' '\0' <<<"$files" | xargs -0 grep -HnIE "$tokens"; tr '\n' '\0' <<<"$files" | xargs -0 grep -HnIiE "$assigned"; } 2>/dev/null | sort -u | cut -c1-160)
[ -n "$bad" ] && problem "Possible passwords, keys or tokens:" "$bad"
bad=$(tr '\n' '\0' <<<"$files" | xargs -0 grep -HnIE "$personal" 2>/dev/null | cut -c1-160)
[ -n "$bad" ] && problem "Personal details (write ~/ instead of /home/<name>/; no personal email):" "$bad"
if [ -n "$words" ]; then
    bad=$(tr '\n' '\0' <<<"$files" | xargs -0 grep -HnIiwF -f <(printf '%s\n' "$words") 2>/dev/null | cut -c1-160)
    [ -n "$bad" ] && problem "Private words (your name, email names or PC user name; list: ~/.config/claude-hub/private-words):" "$bad"
fi

# Code checks (item 24): warnings only, never stop a save. Each runs only if its tool is
# installed (claude-setup's install.sh adds shellcheck and ruff) and only on these files.
note() { echo "check-repo note: $1"; [ -n "$2" ] && sed 's/^/    /' <<<"$2" | head -10; }
sh=$(grep -E '\.sh$' <<<"$files" | while read -r f; do head -1 "$f" 2>/dev/null | grep -q python || echo "$f"; done)   # a .sh that's really Python (statusline.sh) is skipped
if [ -n "$sh" ] && command -v shellcheck > /dev/null; then
    out=$(tr '\n' '\0' <<<"$sh" | xargs -0 shellcheck -S error -f gcc 2>&1) || note "shellcheck found bash mistakes:" "$out"
fi
py=$(grep -E '\.py$' <<<"$files")
if [ -n "$py" ] && command -v ruff > /dev/null; then
    out=$(tr '\n' '\0' <<<"$py" | xargs -0 ruff check -q --select E9,F --output-format concise 2>&1) || note "ruff found Python mistakes:" "$out"
fi
while IFS= read -r j; do
    [ -n "$j" ] && { python3 -m json.tool "$j" > /dev/null 2>&1 || note "not valid JSON:" "$j"; }
done < <(grep -E '\.json$' <<<"$files")
if [ -f docs/STATUS.md ] && grep -qx docs/STATUS.md <<<"$files"; then
    w=$(wc -w < docs/STATUS.md); [ "$w" -gt 800 ] && note "docs/STATUS.md is $w words (aim ~500): move finished items to docs/HISTORY.md." ""
fi

# ---- This project's own rules (add below; call: problem "What it is:" "$bad")

if [ $fail = 0 ]; then
    echo "check-repo: all clear"
else
    echo
    echo "Fix or remove those, then save again. A false alarm? Ask the owner, then"
    echo "add the file's path to tools/check-repo.allow."
fi
exit $fail
