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

echo "Starting the program. Close the program when done."
# Run the program, do not use the '--profile' flag
./conquer_chess

echo "Writing the gprof log to 'gprof.log'."
gprof conquer_chess > gprof.log

echo "Read the 20 top lines of 'gprof.log'" 
head gprof.log -n 20

echo "Speed profile is in 'gprof.log' (as shown above)"
