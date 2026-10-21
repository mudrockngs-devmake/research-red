#!/usr/bin/env bash
# Research Red: archive one playtest round.
#   test-archive/<date>_<version>_<name>/
#     rom/          the exact ROM that was tested
#     screenshots/ saves/ states/ cheats/ patches/ notes/
#                   everything in the six _emu folders (notes/: the testers'
#                   "Save my notes" files from the what's-new report), moved (not deleted)
#                   so each round starts with them empty
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
EMU_FOLDERS="screenshots saves states cheats patches notes"
mkdir -p "$DIR/rom"
for f in $EMU_FOLDERS; do mkdir -p "$DIR/$f"; done

cp "$WS/$ROMFILE" "$DIR/rom/"
# Move (never delete) everything out of the emulator folders; mv -n won't overwrite
for f in $EMU_FOLDERS; do
    [ -d "$EMU/$f" ] || continue
    find "$EMU/$f" -mindepth 1 -maxdepth 1 -exec mv -n {} "$DIR/$f/" \;
    LEFT="$(find "$EMU/$f" -mindepth 1 | wc -l)"
    [ "$LEFT" -eq 0 ] || echo "warning: $LEFT item(s) left in $EMU/$f (name clash?), check by hand" >&2
done

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
| Saves / states / cheats / patches | $(ls "$DIR/saves" | wc -l) / $(ls "$DIR/states" | wc -l) / $(ls "$DIR/cheats" | wc -l) / $(ls "$DIR/patches" | wc -l) |
| Testers' notes | $(ls "$DIR/notes" | wc -l) (notes/, from the what's-new report) |

## Commits in this build
\`\`\`
${LOG:-(none)}
\`\`\`

## Changes to test

## What was tested

## Every-round checklist (docs/PLAYTESTING.md)
- [ ] Intro call to the end, bedroom
- [ ] Player sprite: walk, run, bike, surf
- [ ] Breakfast portraits/name plates; Ivy follows
- [ ] A trainer double ends and the scene continues
- [ ] Guide opens/closes; Save + Continue
- [ ] New-species catch plays the banter
- [ ] No wrong-color flashes on speaker changes

## Observed (bugs, notes)

## Issues filed
(GitHub issue numbers for each problem found, labeled `playtest`)

## Follow-ups

## Session usage
(Claude Code session costs: run `session-cost --all-days` and copy the sessions since the last round)
EOF

mkdir -p "$ARCHIVE"
[ -f "$ARCHIVE/INDEX.md" ] || printf '# Playtest archive\n\n| Date | Build | Test | Folder |\n|---|---|---|---|\n' > "$ARCHIVE/INDEX.md"
echo "| $(date '+%Y-%m-%d') | $VERSION ($COMMIT) | $NAME | $(basename "$DIR") |" >> "$ARCHIVE/INDEX.md"
echo "$CLEAN_COMMIT" > "$ARCHIVE/.last_commit"
echo "$DIR"
