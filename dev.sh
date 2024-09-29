#!/bin/bash

# Exit script if any command fails
set -e
cd build
cmake --build .
./main
