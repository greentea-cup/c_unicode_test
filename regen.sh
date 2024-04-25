#!/bin/sh

export SOURCE="./src/main.c"

export CC=${CC:-clang} CXX=${CXX:-clang}
# export CC=gcc CXX=g++
# msvc is done via VS

BUILD_TYPE="${BUILD_TYPE:-${1:-Release}}"
export EXPORT_COMPILE_COMMANDS="${EXPORT_COMPILE_COMMANDS:-$CCMDS}"
if [ "$EXPORT_COMPILE_COMMANDS" == "0" ]; then
	unset EXPORT_COMPILE_COMMANDS
fi
if [ "$SOURCE" == "" ]; then
    echo "Set SOURCE to your source file to run"
    exit 1
else
	for SOURCE0 in $SOURCE; do
		if [ ! -f "$SOURCE0" ]; then
   			echo "Unknown file '$SOURCE0'"
    		exit 1
		fi
	done
fi
echo "Using BUILD_TYPE=$BUILD_TYPE"
echo "Using EXPORT_COMPILE_COMMANDS=$EXPORT_COMPILE_COMMANDS"
echo "Using SOURCE=$SOURCE"

rm -rf Build compile_commands.json
cmake --preset $BUILD_TYPE

if [ ! -z "$EXPORT_COMPILE_COMMANDS" ]; then
	echo "Making symlink to Build/$BUILD_TYPE/compile_commands.json"
	ln -s Build/$BUILD_TYPE/compile_commands.json .
fi

cmake --build --preset $BUILD_TYPE
./Build/$BUILD_TYPE/app $@

