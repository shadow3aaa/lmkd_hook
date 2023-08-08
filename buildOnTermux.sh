#!/bin/bash

set -e

rm -rf output/*
mkdir -p output

cd output

aarch64-linux-android-clang++ ../inject/inject.cpp -w -O3 -flto -fvisibility=hidden -o inject
aarch64-linux-android-clang++ --shared ../hookLib/hooklib.cpp ../dobby/arm64-v8a/libdobby.a -fPIC -llog -O3 -Wall -o hookLib.so
aarch64-linux-android-strip inject

cp -rf ../module/* .
zip -9 -rq ../lmkd_hook.zip .
echo "Packaged as magisk module: $(realpath ../lmkd_hook.zip)"
