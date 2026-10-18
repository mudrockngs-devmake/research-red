#!/usr/bin/env bash
# Research Red: install the freshly built ROM for testing.
#   - copies pokefirered.gba to the workspace as ResearchRed_local.gba (what mGBA opens)
#   - keeps a versioned copy in builds/ (ResearchRed_v0.0.X_<sha>.gba)
#   - writes ResearchRed_local.txt describing which build is installed
# Usage (from the repo root, after `make firered -j6`): tools/research_red/rr_install_rom.sh
set -euo pipefail

REPO="$(cd "$(dirname "$0")/../.." && pwd)"
WS="/mnt/e/Workspace/romhacks/research-red"
ROM="$REPO/pokefirered.gba"

[ -f "$ROM" ] || { echo "No $ROM; run make firered -j6 first" >&2; exit 1; }

VERSION="$(grep -oE 'v[0-9]+\.[0-9]+\.[0-9]+' "$REPO/src/oak_speech.c" | head -1)"
SHA="$(git -C "$REPO" rev-parse --short HEAD)"
git -C "$REPO" diff --quiet HEAD -- . ':!*.gba' || SHA="$SHA-dirty"
NAME="ResearchRed_${VERSION}_${SHA}"

mkdir -p "$WS/builds"
cp "$ROM" "$WS/builds/$NAME.gba"
cp "$ROM" "$WS/ResearchRed_local.gba"
cat > "$WS/ResearchRed_local.txt" <<EOF
version=$VERSION
commit=$SHA
built=$(date '+%Y-%m-%d %H:%M')
file=builds/$NAME.gba
EOF
echo "Installed $NAME"
