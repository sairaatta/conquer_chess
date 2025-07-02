#!/bin/bash
#
# Get a runtime speed profile of the game.
#
# This script starts the game for you.
# You will need to quit the game.
# Then a runtime speed profile is produced.
#
# The script does the same as
# https://github.com/richelbilderbeek/conquer_chess/blob/main/.github/workflows/get_runtime_speed_profile.yml
#
# Usage:
#
#   ./scripts/get_runtime_speed_profile.sh

if [[ "$PWD" =~ scripts$ ]]; then
    echo "FATAL ERROR."
    echo "Please run the script from the project root. "
    echo "Present working director: $PWD"
    echo " "
    echo "Tip: like this"
    echo " "
    echo "  ./scripts/get_runtime_speed_profile.sh"
    echo " "
    exit 42
fi

cd build/Desktop-Release

# Run the program, do not use the '--profile' flag
./conquer_chess

#  Produce the gprof log
gprof conquer_chess > gprof.log

# Read the 100 top lines
head gprof.log -n 100

