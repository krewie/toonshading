#!/bin/bash

if [ -z "$ASSIGNMENT3_ROOT" ]
then
    echo "ASSIGNMENT3_ROOT is not set"
    exit 1
fi

export PART1_ROOT=$ASSIGNMENT3_ROOT/part1 && \

# Generate build script
cd $PART1_ROOT/build && \
if [ ! -d linux ]; then
    mkdir linux
fi
cd linux && \
cmake -DCMAKE_INSTALL_PREFIX=$PART1_ROOT ../ && \

# Build and install the program
make -j4 && \
make install && \

# Run the program
cd ../../bin && \
./part1