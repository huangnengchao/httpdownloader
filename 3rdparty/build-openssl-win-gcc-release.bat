#!/bin/bash

export OPENSSL_ROOT=`pwd`
export OPENSSL_VERSION="openssl-1.0.2p"

export COMPILER=gcc
export OS=linux
export BUILDTYPE=release
export CONCURRENT_JOB=2

# decompress 
rm -fr $OPENSSL_ROOT/$OPENSSL_VERSION
tar -zxvf $OPENSSL_ROOT/$OPENSSL_VERSION.tar.gz -C $OPENSSL_ROOT

# entry openssl dir
cd $OPENSSL_ROOT/$OPENSSL_VERSION

echo "OPENSSL_ROOT = $OPENSSL_ROOT"

./config --release no-shared --prefix=$OPENSSL_ROOT/../lib/$OS/$COMPILER/$BUILDTYPE/openssl

make -j$CONCURRENT_JOB
make install


set COMPILER=gcc
set OSTYPE=win
set BUILDTYPE=release


cd openssl-1.0.2p
set OPENSSL_ROOT=%cd%

set INSTALL_PATH=%OPENSSL_ROOT%\..\..\lib\%OSTYPE%\%COMPILER%\%BUILDTYPE%\zlib

cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH%

mingw32-make
mingw32-make install


::重命名lib
cd %INSTALL_PATH%/lib

ren libzlibstatic.a libz.a

cd ..