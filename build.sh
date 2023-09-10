#!/bin/sh

set -xe

gcc boxit.c -lSDL2 -lSDL2_gfx -lm -o ./boxit
