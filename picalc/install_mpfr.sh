#!/bin/bash

LIB_MPFR_EXIST=$(/sbin/ldconfig -p | grep "libmpfr.so")

if [ "$LIB_MPFR_EXIST" == "" ]; then
    sudo apt-get install -y libmpfr-dev
fi
