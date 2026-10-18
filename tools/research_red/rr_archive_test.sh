#!/usr/bin/env bash
# Research Red: archive one playtest round.
#   test-archive/<date>_<version>_<name>/
#     rom/          the exact ROM that was tested
#     screenshots/  moved out of the emulator folder (so it starts empty next round)
#     saves/ states/ copied (the emulator keeps its own so play can continue)
#     NOTES.md      build info, commits in this build, then test notes to fill in
#   test-archive/INDEX.md gets one line per round.
# Usage: tools/research_red/rr_archive_test.sh <short-test-name>
set -euo pipefail

REPO="$(cd "$(dirname "$0")/../.." && pwd)"
WS="/mnt/e/Workspace/romhacks/research-red"
EMU="/mnt/e/Workspace/romhacks/_emu"
ARCHIVE="$WS/test-archive"
NAME="${1:?usage: rr_archive_test.sh <short-test-name>}"
NAME="$(echo "$NAME" | tr ' ' '-' | tr -cd 'A-Za-z0-9._-')"

INFO="$WS/ResearchRed_local.txt"
[ -f "$INFO" ] || { echo "No $INFO; install the ROM with rr_install_rom.sh first" >&2; exit 1; }
VERSION="$(grep '^version=' "$INFO" | cut -d= -f2)"
COMMIT="$(grep '^commit=' "$INFO" | cut -d= -f2)"
BUILT="$(grep '^built=' "$INFO" | cut -d= -f2)"
ROMFILE="$(grep '^file=' "$INFO" | cut -d= -f2)"

DIR="$ARCHIVE/$(date '+%Y-%m-%d')_${VERSION}_${NAME}"
[ -e "$DIR" ] && { echo "$DIR already exists" >&2; exit 1; }
mkdir -p "$DIR/rom" "$DIR/screenshots" "$DIR/saves" "$DIR/states"

cp "$WS/$ROMFILE" "$DIR/rom/"
find "$EMU/screenshots" -maxdepth 1 -type f -exec mv {} "$DIR/screenshots/" \;
find "$EMU/saves" -maxdepth 1 -type f -name 'ResearchRed*' -exec cp {} "$DIR/saves/" \;
find "$EMU/states" -maxdepth 1 -type f -name 'ResearchRed*' -exec cp {} "$DIR/states/" \;

# Commits since the previous archived round (or the last 10 on the first round)
PREV="$(cat "$ARCHIVE/.last_commit" 2>/dev/null || true)"
CLEAN_COMMIT="${COMMIT%-dirty}"
if [ -n "$PREV" ] && git -C "$REPO" cat-file -e "$PREV" 2>/dev/null; then
    LOG="$(git -C "$REPO" log --oneline "$PREV..$CLEAN_COMMIT")"
else
    LOG="$(git -C "$REPO" log --oneline -10 "$CLEAN_COMMIT")"
fi

cat > "$DIR/NOTES.md" <<EOF
# Playtest: $NAME

| | |
|---|---|
| Build | $VERSION ($COMMIT), built $BUILT |
| ROM | rom/$(basename "$ROMFILE") |
| Archived | $(date '+%Y-%m-%d %H:%M') |
| Screenshots | $(ls "$DIR/screenshots" | wc -l) |

## Commits in this build
\`\`\`
${LOG:-(none)}
\`\`\`

## Changes to test

## What was tested

## Observed (bugs, notes)

## Follow-ups

## Session usage
(Claude Code session cost/tokens from /cost, if recorded)
EOF

mkdir -p "$ARCHIVE"
[ -f "$ARCHIVE/INDEX.md" ] || printf '# Playtest archive\n\n| Date | Build | Test | Folder |\n|---|---|---|---|\n' > "$ARCHIVE/INDEX.md"
echo "| $(date '+%Y-%m-%d') | $VERSION ($COMMIT) | $NAME | $(basename "$DIR") |" >> "$ARCHIVE/INDEX.md"
echo "$CLEAN_COMMIT" > "$ARCHIVE/.last_commit"
echo "$DIR"
