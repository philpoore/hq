#!/bin/bash

LIB_DIR="$PWD/lib"
URL="https://github.com/CLIUtils/CLI11/releases/download/v2.5.0/CLI11.hpp"

mkdir $LIB_DIR

curl -L $URL -o $LIB_DIR/CLI11.hpp