# mine sweeper in C++

inspired from ["coding train challenge"](https://thecodingtrain.com/CodingChallenges/071-minesweeper.html)

use a canvas , a setup and a draw... but in C++

## application.h
mimic p5.js canvas application 

## qtcanvas.h
implements a canvas with Qt5 widget lib 

# minesweeper.cpp
implements the minesweeper application

# how to build

## dependancies
Qt5
Cmake > 3.0
c++17 compiler

## build and run steps (tested on ubuntu 16.04)
```
cd /path/to/this/directory
mkdir build && cd build
cmake CMAKE_PREFIX_PATH=/path/to/qt/lib ..
cmake --build . --target all

./minesweeper
```
