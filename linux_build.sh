#!/bin/bash


# 此脚本仅仅用来编译linux 下工程

cd ./build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make clean
make
cd server
cp -r ../../conf .
mkdir logs

