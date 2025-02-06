#!/bin/bash

total=10

if [ $# -eq 0 ]; then
    echo "No arguments were given, default value is $total."
else
    total=$1
fi

for ((i = 1; i <= total; i++))
do
    IDADE=$(((RANDOM % 100)+1))
    echo "./people -i Pessoa${i} $IDADE"

    ./people -i Pessoa${i} $IDADE

done
