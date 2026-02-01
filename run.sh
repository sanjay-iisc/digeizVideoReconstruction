#! /bin/sh

./config.sh
./build.sh
cd build ; export OMP_NUM_THREADS=24; ./videoReorder

# ./videoToImage