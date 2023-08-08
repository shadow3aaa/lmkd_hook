#!/bin/bash

set -e
cd output

aarch64-linux-android-clang++ ../inject/inject.cpp -w -O3 -flto -o inject
aarch64-linux-android-clang++ --shared ../hookLib/hooklib.cpp ../dobby/arm64-v8a/libdobby.a -fPIC -llog -O3 -Wall -o hookLib.so

cp -rf ../module/* .
zip -9 -rq ../lmkd_hook.zip .
echo "Packaged as magisk module: $(realpath ../lmkd_hook.zip)"