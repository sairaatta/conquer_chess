# This is a copy of game.pro,
# so that Saraj can work on that one without me being in the way.
#
# The goal of this project file
# is to create a static version
# for Linux only.

# From https://stackoverflow.com/a/20799203/3364162
CONFIG += static
CONFIG += staticlib

# From https://stackoverflow.com/a/17712841/3364162
CONFIG += static-libgcc
CONFIG += static-libstdc++

# From https://doc.qt.io/qt-6/qmake-advanced-usage.html
CONFIG += create_prl
CONFIG += link_prl

# Do have the Qt source files installed.
# You can check this in the maintenance tool
# at ~/Qt/MaintenanceTool
# 
# To find the Qt maintenance tool, from https://askubuntu.com/a/898189/1757050 :
#
# sudo apt-get install mlocate
# sudo updatedb
# locate -i maintenance | grep -i qt


# All files are in here, the rest are just settings
include(game.pri)
include(game_view.pri)

TARGET = conquer_chess_linux_static

# Use the C++ version that all team members can use
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

# High warning levels
QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic

# Debug and release settings
CONFIG += debug_and_release
CONFIG(release, debug|release) {

  message("Compiling in release mode")

  DEFINES += NDEBUG

  # gprof
  QMAKE_CXXFLAGS += -pg
  QMAKE_LFLAGS += -pg
}

CONFIG(debug, debug|release) {

  message("Compiling in debug mode")

  # High warning levels
  QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic

  # A warning is an error
  QMAKE_CXXFLAGS += -Werror

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov
}

# Qt5
QT += core gui widgets

# SFML
# LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# SFGraphing
DEFINES += USE_SFGRAPHING
SOURCES += $$PWD/../SFGraphing/src/*.cpp
INCLUDEPATH += $$PWD/../SFGraphing/include
HEADERS += $$PWD/../SFGraphing/include/SFGraphing

