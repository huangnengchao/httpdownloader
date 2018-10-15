#!/bin/bash
export ZLIB_ROOT="$( cd "$(dirname "$0")" ; pwd -P )"

export COMPILER=gcc
export OS=linux
export BUILDTYPE=debug
export CONCURRENT_JOB=2

rm -fr $ZLIB_ROOT/zlib-1.2.11
tar -xzvf $ZLIB_ROOT/zlib-1.2.11.tar.gz -C $ZLIB_ROOT

cd $ZLIB_ROOT/zlib-1.2.11

echo ZLIB_ROOT=$ZLIB_ROOT
export CPLUS_INCLUDE_PATH="/usr/include/x86_64-linux-gnu"  
export C_INCLUDE_PATH="/usr/include/x86_64-linux-gnu" 

echo "ZLIB_ROOT = $ZLIB_ROOT"

./configure  --prefix=$ZLIB_ROOT/../lib/$OS/$COMPILER/$BUILDTYPE/zlib  --static
make -j${CONCURRENT_JOB}
make install
