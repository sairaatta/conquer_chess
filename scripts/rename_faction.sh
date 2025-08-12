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
    echo "  ./scripts/rename_faction.sh xayid spawn"
    echo " "
    exit 42
fi

if [ "$#" -ne 2 ]; then
    echo "Please use 2 arguments, for example: "
    echo " "
    echo "  ./scripts/rename_faction.sh xayid spawn"
    echo " "
    echo "Number of arguments used: $#"
fi

from_faction=$1
to_faction=$2

echo "From (lowercase): ${from_faction}"
echo "To (lowercase): ${to_faction}"

from_uppercase_faction=${from_faction^}
to_uppercase_faction=${to_faction^}

echo "From (uppercase): ${from_uppercase_faction}"
echo "To (uppercase): ${to_uppercase_faction}"

# Allows rename to use **
# Thanks to pa4080, from
# https://askubuntu.com/questions/1287359/how-to-use-the-command-rename-on-subdirectories-as-well#1287371
shopt -s globstar
rename "s/${from_faction}/${to_faction}/" **/*

#rename --nono 's/xayid/spawn/' **/*
#rename --nono 's/genetron/kingdom/' **/*
#rename --nono 's/keiron/rooxx/' **/*

# find . -not -path '*/\.git/*' -name '*.rb'  -exec sed -i 's/old_string/new_string/g' '{}' \;
# find . -not \( -path '*/\.git/*' -path '*/\.github/*' -path '*/build/*'  \) -exec sed -i 's/old_string_wiefgiewguwfe/new_string/g' '{}' \;
# find . -not \( -path '*/\.git/*' -path '*/\.github/*' -path '*/build/*'  \) \( -name "*.md" -o -name "*.cpp" -o -name "*.h" -o -name "*.qrc" \)
find . -not \( -path '*/\.git/*' -path '*/\.github/*' -path '*/build/*'  \) \( -name "*.md" -o -name "*.cpp" -o -name "*.h" -o -name "*.qrc" \) -exec sed -i "s/${from_faction}/${to_faction}/" '{}' \;
