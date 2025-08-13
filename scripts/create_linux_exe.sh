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

qmake game_linux_static.pro
make
