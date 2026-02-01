#! /bin/sh

./config.sh
./build.sh
cd build ; OMP_NUM_THREADS=$(nproc); ./videoToImage; ./videoReorder
