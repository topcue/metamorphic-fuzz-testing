#!/bin/bash

## Config def.h
echo "#pragma once" > src/def.h
echo "#define ADDRESS_SANITIZER" >> src/def.h

## Config Makefile components
export TARGET="asan"
export CFLAGS="-fsanitize=address"

## Build
cd src && make && cp asan .. && make clean

# EOF
