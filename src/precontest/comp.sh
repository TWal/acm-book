#!/bin/bash
if g++ -DDEBUG -std=c++14 -g -Wall -Wextra $1.cpp -o $1; then
    for i in $1.in*; do
        echo $i
        time ./$1 < $i
    done
fi


