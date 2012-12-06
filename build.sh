#!/bin/sh -e

mkdir -p bin

if [ "$1" == "clean" ]
then
  echo "Cleaning"
  cd bin
  make clean
  cd ..
fi

cd bin
cmake ..
make

