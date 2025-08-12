#!/bin/bash
#
# Rename factions.
#
# Usage:
#
#   ./scripts/rename_faction.sh xayid spawn
#   

if [[ "$PWD" =~ scripts$ ]]; then
    echo "FATAL ERROR."
    echo "Please run the script from the project root. "
    echo "Present working director: $PWD"
    echo " "
    echo "Tip: like this"
    echo " "
    echo "  ./scripts/fix_markdown_style_errors.sh"
    echo " "
    exit 42
fi

rename --nono 's/xayid/spawn/' *
