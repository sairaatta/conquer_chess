#!/bin/bash
#
# Build using CMake
#
# Usage:
#
#   ./scripts/build_with_cmake.sh

if [[ "$PWD" =~ scripts$ ]]; then
    echo "FATAL ERROR."
    echo "Please run the script from the project root. "
    echo "Present working director: $PWD"
    echo " "
    echo "Tip: like this"
    echo " "
    echo "  ./scripts/build_with_cmake.sh"
    echo " "
    exit 42
fi

# From https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html
mkdir build_cmake
cd build_cmake
cmake ../
cmake --build .

