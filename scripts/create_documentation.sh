#!/bin/bash
#
# Creates the Doxygen documentation.
#
# Usage:
#
#   ./scripts/create_documentation.sh

if [[ "$PWD" =~ scripts$ ]]; then
    echo "FATAL ERROR."
    echo "Please run the script from the project root. "
    echo "Present working director: $PWD"
    echo " "
    echo "Tip: like this"
    echo " "
    echo "  ./scripts/create_documentation.sh"
    echo " "
    exit 42
fi

doxygen Doxyfile
