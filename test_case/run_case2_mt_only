#!/bin/bash

## Config def.h
echo "#pragma once" > src/def.h
echo "#define METAMORPHIC_TESTING" >> src/def.h

## Config Makefile components
export TARGET="case2"
export CFLAGS="-g -Wall -O3"

## Build & Run
cd src && make && cp case2 .. && make clean
cd .. && ./case2

# EOF
