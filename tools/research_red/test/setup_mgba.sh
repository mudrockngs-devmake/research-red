#!/usr/bin/env bash
# Research Red: build the headless test runner (rr_run) once per machine.
#
# Downloads CMake and the mGBA 0.10.5 source into $RR_TOOLS (default
# ~/romhacks/_tools, outside the repo), builds mGBA as a library with no
# window/sound/Qt, and compiles tools/research_red/test/rr_run.c against it.
# Needs gcc, make, curl, libpng and zlib headers (already used by the ROM build).
#
# Usage: tools/research_red/test/setup_mgba.sh
# Result: $RR_TOOLS/rr_run   (smoke.py finds it there)
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
TOOLS="${RR_TOOLS:-$HOME/romhacks/_tools}"
CMAKE_VER=3.30.5
MGBA_VER=0.10.5
mkdir -p "$TOOLS"
cd "$TOOLS"

if [ ! -x "cmake-$CMAKE_VER-linux-x86_64/bin/cmake" ]; then
    echo "Downloading CMake $CMAKE_VER..."
    curl -sL -o cmake.tgz "https://github.com/Kitware/CMake/releases/download/v$CMAKE_VER/cmake-$CMAKE_VER-linux-x86_64.tar.gz"
    tar xzf cmake.tgz && rm cmake.tgz
fi
CMAKE="$TOOLS/cmake-$CMAKE_VER-linux-x86_64/bin/cmake"

if [ ! -f "mgba-$MGBA_VER/build/libmgba.a" ]; then
    echo "Downloading and building mGBA $MGBA_VER (a few minutes)..."
    [ -d "mgba-$MGBA_VER" ] || { curl -sL -o mgba.tgz "https://github.com/mgba-emu/mgba/archive/refs/tags/$MGBA_VER.tar.gz"; tar xzf mgba.tgz && rm mgba.tgz; }
    mkdir -p "mgba-$MGBA_VER/build"
    cd "mgba-$MGBA_VER/build"
    "$CMAKE" .. -DBUILD_QT=OFF -DBUILD_SDL=OFF -DUSE_FFMPEG=OFF -DUSE_LUA=OFF -DUSE_EPOXY=OFF \
        -DBUILD_GL=OFF -DBUILD_GLES2=OFF -DBUILD_GLES3=OFF -DUSE_SQLITE3=OFF -DUSE_ELF=OFF \
        -DUSE_LIBZIP=OFF -DUSE_MINIZIP=OFF -DUSE_DISCORD_RPC=OFF -DUSE_EDITLINE=OFF \
        -DBUILD_SHARED=OFF -DBUILD_STATIC=ON -DM_CORE_GB=OFF -DUSE_DEBUGGERS=OFF \
        -DCMAKE_BUILD_TYPE=Release > cmake.log 2>&1
    make -j"$(nproc)" mgba > build.log 2>&1
    cd "$TOOLS"
fi

echo "Compiling rr_run..."
gcc -O2 -include mgba/flags.h -o "$TOOLS/rr_run" "$HERE/rr_run.c" \
    -I"mgba-$MGBA_VER/include" -I"mgba-$MGBA_VER/build/include" \
    "mgba-$MGBA_VER/build/libmgba.a" -lpng -lz -lm -lpthread
echo "Ready: $TOOLS/rr_run"
