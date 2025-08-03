#!/bin/bash
#
# Create the Conquer Chess Windows executable.
#
# Usage:
#
#   ./scripts/create_windows_exe.sh

if [[ "$PWD" =~ scripts$ ]]; then
    echo "FATAL ERROR."
    echo "Please run the script from the project root. "
    echo "Present working director: $PWD"
    echo " "
    echo "Tip: like this"
    echo " "
    echo "  ./scripts/create_windows_exe.sh"
    echo " "
    exit 42
fi

#Don't put the full path here. It has to be found in the 
# In /home/yourusername/.bashrc add the following line add the end instead:
# export PATH=$PATH:/home/richel/GitHubs/RibiLibraries/Libraries/mxe/usr/bin/"
PATH="$PATH:/home/richel/GitHubs/RibiLibraries/Libraries/mxe/usr/bin/"
echo $PATH


myqmake="i686-w64-mingw32.static-qmake-qt5"
echo "Displaying cross compiler version (if it is not found, set the path to YourFolder/mxe/usr/bin"
$myqmake -v

# Don't add '-qt=qt5', as myqmake already IS Qt5
$myqmake game.pro


if [ ! -e Makefile ]
then
  echo "FAIL:"$myqmake", "$myprofile": FAIL (Makefile not found)"
  exit 1
fi

echo "Start make"

make -s

if [ -e ./release/conquer_chess.exe ]
then
  echo "conquer_chess.exe: SUCCESS"
  if [ -e ./release/conquer_chess.exe ]
  then
    cp ./release/conquer_chess.exe .
  fi
else
  echo $myprofile", "$mytypestr": FAIL (executable not found)"
fi
