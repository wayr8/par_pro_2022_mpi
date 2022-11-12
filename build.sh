#!/bin/bash
mkdir build
cd build
cmake -DUSE_SEQ=OFF -DUSE_MPI=ON -DUSE_STD=OFF -DUSE_OMP=OFF -DUSE_TBB=OFF -DUSE_STYLE_CHECKER=OFF ..
cmake --build . --config RELEASE