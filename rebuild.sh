#!/bin/bash

cd ./libs/picotls
make clean
make
rm -f libpicotls-fusion.a

cd ../../libs/picoquic
make clean
make

cd ../..