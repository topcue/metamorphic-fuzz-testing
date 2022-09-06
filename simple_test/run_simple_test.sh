#!/bin/bash

## Set base workspace
export BASE=$HOME/metamorphic-fuzz-testing

## Build PUT w/ libfuzzer
$BASE/clang-llvm/bin/clang -g -fsanitize-coverage=trace-pc-guard test_fuzzer.cpp -c

## Build afl-llvm-rt.o.c from the AFL
$BASE/clang-llvm/bin/clang -c -w $BASE/afl/llvm_mode/afl-llvm-rt.o.c

## Build afl_driver.cpp & link it w/ afl-llvm-rt.o.o and PUT
$BASE/clang-llvm/bin/clang++ $BASE/compiler-rt/lib/fuzzer/afl/afl_driver.cpp test_fuzzer.o afl-llvm-rt.o.o -o test_fuzzer

## Run w/ afl
rm -rf in out; mkdir in out; echo z > in/z;
$BASE/afl/afl-fuzz -i in -o out ./test_fuzzer

# EOF

