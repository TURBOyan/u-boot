#!/bin/sh

export ARCH=arm
export CROSS_COMPILE=/opt/toolchain/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-


make -j8
