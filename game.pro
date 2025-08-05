# This is the general project file,
# to be used to simply run the game.
#
# Other .pro files are used for specific tasks,
# such as codecov or profiling

# On GHA, this DEFINE is added by the .yaml script
#
#DEFINES += LOGIC_ONLY


# All files are in here, the rest are just settings
include(game.pri)
include(game_view.pri)

TARGET = conquer_chess

# Use the C++ version that all team members can use
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

# High warning levels
QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic

# Debug and release settings
CONFIG += debug_and_release
CONFIG(release, debug|release) {
  DEFINES += NDEBUG

  # gprof
  QMAKE_CXXFLAGS += -pg
  QMAKE_LFLAGS += -pg
}

CONFIG(debug, debug|release) {
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
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio


win32 {

  LIBS += -L/$$PWD/../SFML/lib
  INCLUDEPATH += -I/$$PWD/../SFML/include

}

# Crosscompile notes
#CFLAGS += SFML_USE_STATIC_STD_LIBS=TRUE
#CFLAGS += -DSFML_STATIC
#LIBs += -L../RibiLibraries/mxe/usr/i686-w64-mingw32.static/lib
#LIBS += -lsfml
# LIBS += -L/home/richel/GitHubs/RibiLibraries/mxe/usr/i686-w64-mingw32.static/lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s
#LIBs += -l/home/richel/GitHubs/RibiLibraries/mxe/usr/i686-w64-mingw32.static/lib/sfml-graphics
#LIBs += -l/home/richel/GitHubs/RibiLibraries/mxe/usr/i686-w64-mingw32.static/lib/sfml-window
#LIBs += -l/home/richel/GitHubs/RibiLibraries/mxe/usr/i686-w64-mingw32.static/lib/sfml-system
#LIBs += -l/home/richel/GitHubs/RibiLibraries/mxe/usr/i686-w64-mingw32.static/lib/sfml-audio

# SFGraphing
DEFINES += USE_SFGRAPHING
SOURCES += $$PWD/../SFGraphing/src/*.cpp
INCLUDEPATH += $$PWD/../SFGraphing/include
HEADERS += $$PWD/../SFGraphing/include/SFGraphing
