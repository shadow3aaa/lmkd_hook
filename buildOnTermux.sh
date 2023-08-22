#!/bin/bash

set -e

rm -rf output/*
mkdir -p output

cd output

clang++ ../inject/inject.cpp -w -O3 -static-libstdc++ -flto -fvisibility=hidden -o inject
clang++ --shared ../hookLib/hooklib.cpp ../dobby/arm64-v8a/libdobby.a -fPIC -static-libstdc++ -llog -O3 -Wall -fvisibility=hidden -o hookLib.so

strip inject
strip hookLib.so

cp -rf ../module/* .
zip -9 -rq ../lmkd_hook.zip .
echo "Packaged as magisk module: $(realpath ../lmkd_hook.zip)"
