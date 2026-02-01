#!/bin/sh
echo "configuring cmake ....."

folderName="build"

if [ -d "$folderName" ]; then
    echo "$folderName is already there.."
    rm -rf build
fi

mkdir -p build

echo "$folderName is created"

cmake -S. -B build

