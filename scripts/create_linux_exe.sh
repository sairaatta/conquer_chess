#!/bin/bash
#
# Create the Conquer Chess Linux standalone executable.
#
# Usage:
#
#   ./scripts/create_linux_exe.sh

if [[ "$PWD" =~ scripts$ ]]; then
    echo "FATAL ERROR."
    echo "Please run the script from the project root. "
    echo "Present working director: $PWD"
    echo " "
    echo "Tip: like this"
    echo " "
    echo "  ./scripts/create_linux_exe.sh"
    echo " "
    exit 42
fi

qmake6 game_linux_static.pro

makefile_path=Makefile
if [ ! -f ${makefile_path} ]; then
    echo "ERROR: file '${makefile_path}' not created."
    exit 42
fi

make debug

exe_path=release/conquer_chess_linux_static
if [ ! -f ${exe_path} ]; then
    echo "ERROR: file '${exe_path}' not created."
    exit 42
fi

exe_file_size=$(stat -c%s ${exe_path})

if [ ${exe_file_size} -lt 200000000 ]; then
    echo "WARNING: file '${exe_path}' is smaller than 200MB."
    echo "This probably means that the file is build dynamically"
fi
