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

# Thanks to pa4080, from
# https://askubuntu.com/questions/1287359/how-to-use-the-command-rename-on-subdirectories-as-well#1287371
shopt -s globstar

rename --nono 's/xayid/spawn/' **/*
rename --nono 's/genetron/kingdom/' **/*
rename --nono 's/keiron/rooxx/' **/*
