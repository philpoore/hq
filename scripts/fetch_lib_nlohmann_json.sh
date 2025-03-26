#!/bin/bash

LIB_DIR="$PWD/lib"
URL="https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp"

mkdir $LIB_DIR

curl -L $URL -o $LIB_DIR/nlohmann_json.hpp