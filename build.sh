#!/bin/bash

if [ ! -d "build" ]; then
    echo "mkdir build"
    mkdir build
fi

echo "cd build"
cd build

echo "cmake .."
cmake ..

echo "make"
make

