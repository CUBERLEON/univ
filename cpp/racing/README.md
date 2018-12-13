# Racing

## Description:
Racing simulator that involves networking (TCP), strategies (bots), console drawing (curses), threading, unit testing (catch).

## Build intructions:
### Windows:
- Install <a href="https://sourceforge.net/projects/mingw-w64/">MinGW-w64</a>
- Build <a href="https://github.com/Bill-Gray/PDCurses">pdcurses</a> library and then add it to MinGW
- Use `mingw32-make` to build
### Linux:
- Install `ncurses` e.g. `sudo apt-get install libncurses5-dev`
- Install `gpm` e.g. `sudo apt-get install libgpm-dev`
- Use `make` to build
