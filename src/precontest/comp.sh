#!/bin/bash
if g++ -DDEBUG -D_GLIBCXX_DEBUG -std=c++14 -g -Wall -Wextra $1.cpp -o $1; then
    for i in $1.in*; do
        echo $i
        valgrind -q ./$1 < $i
    done
fi


