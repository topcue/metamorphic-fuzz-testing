#!/bin/bash

## Set base workspace
export BASE=$HOME/metamorphic-fuzz-testing

## Get clang-llvm (released version)
cd $BASE
wget https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz
tar -xf clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz
rm clang+llvm*.tar.xz && mv clang+llvm* clang-llvm

## Get compiler-rt-llvm (released version)
cd $BASE
wget https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/compiler-rt-13.0.0.src.tar.xz
tar -xf compiler-rt*tar.xz
rm compiler-rt*tar.xz && mv compiler-rt* compiler-rt

## Build afl (pure afl)
cd $BASE
wget http://lcamtuf.coredump.cx/afl/releases/afl-latest.tgz
tar -xvf afl-latest.tgz
rm afl-latest.tgz && mv afl-2*/ afl

cd $BASE/afl && make

# EOF
