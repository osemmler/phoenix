#!/bin/bash

git pull
git submodule init
git submodule update
cd ../src
qmake
make
