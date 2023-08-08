#!/bin/bash

wget --no-check-certificate 'https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz'
tar xf boost_1_82_0.tar.gz
cd boost_1_82_0
./bootstrap.sh

# Compile with fPIC flag
export CXXFLAGS="-fPIC"
./b2 cxxflags="$CXXFLAGS" link=static
