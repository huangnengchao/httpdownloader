#!/bin/bash
export CURL_ROOT="$( cd "$(dirname "$0")" ; pwd -P )"
export OPENSSL_ROOT="$( cd "$(dirname "$0")" ; pwd -P )"
export ZLIB_ROOT="$( cd "$(dirname "$0")" ; pwd -P )"

export COMPILER=clang
export OS=mac
export BUILDTYPE=release
export CONCURRENT_JOB=2

rm -fr $CURL_ROOT/curl-7.61.1
tar -xzvf $CURL_ROOT/curl-7.61.1.tar.gz -C $CURL_ROOT

cd $CURL_ROOT/curl-7.61.1

echo CURL_ROOT=$CURL_ROOT
export CPLUS_INCLUDE_PATH="/usr/include/x86_64-linux-gnu"  
export C_INCLUDE_PATH="/usr/include/x86_64-linux-gnu" 

echo "CURL_ROOT = $CURL_ROOT"

./configure  --prefix=$CURL_ROOT/../lib/$OS/$COMPILER/$BUILDTYPE/curl  --enable-debug=no --enable-static=yes --enable-shared=no \
 --enable-http --with-ssl=$OPENSSL_ROOT/../lib/$OS/$COMPILER/$BUILDTYPE/openssl --with-zlib=$ZLIB_ROOT/../lib/$OS/$COMPILER/$BUILDTYPE/zlib
make -j${CONCURRENT_JOB}
make install