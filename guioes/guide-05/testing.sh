#|/bin/bash


for i in $(seq 0 9); do
    ./client "$i"
done

