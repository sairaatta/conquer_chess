#!/bin/bash
#
# Run steamcmd to create a new build
#
#
echo "Creating new build"

steamcmd +login richelbilderbeek +run_app_build /home/richel/GitHubs/conquer_chess/steam/app_build_3873940.vdf

echo "Activate the new build on Steamworks."
read -p "Press enter when done"

echo "Start the game via Steam."
read -p "Press enter when done"

echo "Checking Steam install."
./scripts/check_steam_install.sh
