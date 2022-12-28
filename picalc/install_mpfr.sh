#!/bin/bash

APT_PKG=libmpfr-dev
LIB_MPFR_EXIST=$(apt list --installed | grep $APT_PKG)
SUDO=$(which sudo)

if [ "$LIB_MPFR_EXIST" == "" ]; then
    $SUDO apt-get install -y $APT_PKG
fi
