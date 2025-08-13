#!/bin/bash

if ! command -v qmake2cmake >/dev/null 2>&1
then
    echo "'qmake2cmake' could not be found. "
    echo "Installing it ruthlessly ..."
    python -m pip install qmake2cmake --break-system-packages
fi

if ! command -v qmake2cmake >/dev/null 2>&1
then
    echo "ERROR: 'qmake2cmake' could still not be found...?"
    exit 42
fi

qt_version=$(qmake6 --version | grep -o "Qt version .* in " | grep -o "[0-9]\\.[0-9]\\.[0-9]")
echo "Qt version: ${qt_version}"

# qmake2cmake

