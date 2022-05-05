#!/bin/bash

## Config def.h
echo "#pragma once" > src/def.h
echo "#define METAMORPHIC_FUZZ_TESTING" >> src/def.h

## Build PUT w/ libfuzzer
$HOME/metamorphic-fuzz-testing/clang-llvm/bin/clang -g -fsanitize-coverage=trace-pc-guard src/main.c src/KISA_SHA256.c src/Metamorphic_Fuzz_Test.c -c

## Build afl-llvm-rt.o.o from the AFL
$HOME/metamorphic-fuzz-testing/clang-llvm/bin/clang -c -w $HOME/metamorphic-fuzz-testing/afl/llvm_mode/afl-llvm-rt.o.c

## Build afl_driver & link it w/ afl-llvm-rt.o.o and PUT
$HOME/metamorphic-fuzz-testing/clang-llvm/bin/clang++ $HOME/metamorphic-fuzz-testing/compiler-rt/lib/fuzzer/afl/afl_driver.cpp main.o KISA_SHA256.o Metamorphic_Fuzz_Test.o afl-llvm-rt.o.o -o case4

## Clean
rm afl-llvm-rt.o.o KISA_SHA256.o Metamorphic_Fuzz_Test.o main.o

## Run w/ afl
rm -rf in out; mkdir in out; echo z > in/z;
$HOME/metamorphic-fuzz-testing/afl/afl-fuzz -i in -o out ./case4

# EOF

