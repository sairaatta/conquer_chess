---
tags:
  - build
  - run
---

# Build and run

!!! info "This guide assumes Ubuntu"

    Conquer Chess is tested to build correctly under Ubuntu, using
    [the `check_build` script](https://github.com/richelbilderbeek/conquer_chess/blob/main/.github/workflows/check_build.yaml).

    Feel encouraged to [contribute](contributing.md) by writing this
    guide for another operating system or Linux distribution.

## Install prerequisites

Install git:

```console
sudo apt install git
```

Install Qt6:

```console
sudo apt install qt6-base-dev

# Maybe too:
# - qt6-base-dev-tools
# - qt6-tools-dev
# - qt6-tools-dev-tools
# - qt6-declarative-dev
# - qt6-quick3d-dev
# - qt6-sensors-dev
```

Install SFML:

```console
sudo apt install libsfml-dev
```

## Download code

Navigate to your favorite folder for games,
where you must download **two** code repositories.

In your games folder, download the Conquer Chess code:

```console
git clone https://github.com/richelbilderbeek/conquer_chess.git
```

In your games folder, download the `magic_enum` code:

```console
git clone https://github.com/Neargye/magic_enum
```

## Build

To build Conquer Chess, do, in the root of this repository:

```console
qmake game.pro
make release
```

You can now run the game with:

```console
./conquer_chess
```

