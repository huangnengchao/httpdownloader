set COMPILER=gcc
set OSTYPE=win
set BUILDTYPE=release


cd zlib-1.2.11
set ZLIB_ROOT=%cd%

set INSTALL_PATH=%ZLIB_ROOT%\..\..\lib\%OSTYPE%\%COMPILER%\%BUILDTYPE%\zlib

cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH%

mingw32-make
mingw32-make install


::重命名lib
cd %INSTALL_PATH%/lib

ren libzlibstatic.a libz.a

cd ..


