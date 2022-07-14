#!/bin/bash

cd ./libs/picotls
make clean
make

cd ../../libs/picoquic
make clean
make

cd ../..