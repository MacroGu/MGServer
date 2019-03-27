#!/bin/bash


# 此脚本仅仅用来编译linux 下工程

cd librdkafka/linux
./configure
make clean
make
cd ../../build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
cd server
cp -r ../../conf .
mkdir logs

