#!/usr/bin/env bash

# intall packages
brew install fftw
brew install sound-touch
brew install libogg
brew install libvorbis

mkdir build
cd build
mkdir installs
MACOSX_DEPLOYMENT_TARGET=10.14
export MACOSX_DEPLOYMENT_TARGET
cmake -D CMAKE_PREFIX_PATH=/opt/homebrew/opt/qt@5 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./installs ../
make -j 2
make install
open installs/nootka.app
