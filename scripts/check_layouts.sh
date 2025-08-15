#!/bin/bash
#
# Check the layouts of the game dialogs
#
# Usage:
#
#   ./scripts/check_layouts.sh

if [[ "$PWD" =~ scripts$ ]]; then
    echo "FATAL ERROR."
    echo "Please run the script from the project root. "
    echo "Present working director: $PWD"
    echo " "
    echo "Tip: like this"
    echo " "
    echo "  ./scripts/check_layouts.sh"
    echo " "
    exit 42
fi

./scripts/convert_wkt_to_png.py build/Desktop-Debug/tmp_game_view_layout.wkt game_view_layout.png
./scripts/convert_wkt_to_png.py build/Desktop-Debug/tmp_game_view_layout_1600_x_900.wkt game_view_layout_1600_x_900.png
