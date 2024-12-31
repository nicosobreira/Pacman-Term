#!/usr/bin/env bash

[[ ! -d ./build ]] && mkdir ./build
cd build
cmake ../
make && ./pacman
