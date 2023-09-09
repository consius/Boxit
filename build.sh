#!/bin/sh

set -xe

mkdir -p build
gcc boxit.c -lSDL2 -lSDL2_gfx -lm -o ./build/boxit
