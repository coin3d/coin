#!/bin/env sh

cd $(dirname $0)

#Runs necessary generation commands if run from this directory
./inventify_texturenames.sh ../all-profiler-cpp.cpp ../../../data/profiler/*.svg
