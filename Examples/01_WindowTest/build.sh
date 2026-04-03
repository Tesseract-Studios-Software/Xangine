#!/bin/bash

# Build Xangine library first
cd ../..
mkdir -p build
cd build
cmake ..
make

# Build the example
cd ../Examples/01_WindowTest
mkdir -p build
cd build
cmake ..
make

# Run
./WindowTest