#!/bin/bash

# Mutate the file
for ((i=1; i<=1; i++)); do
    val=$(bc <<< "scale=1; $i / 10")
    ./mutate -i example/chry.txt -o example/chry_0"$val".txt -p "$val"
done

# Compress the files
for ((i=1; i<=9; i++)); do
    val=$(bc <<< "scale=1; $i / 10")
    tar -cJf example/chry_0"$val".tar.xz example/chry_0"$val".txt
    echo "Tar done"
    gzip -c example/chry_0"$val".txt > example/chry_0"$val".gz
    echo "Gzip done"
    bzip2 -c example/chry_0"$val".txt > example/chry_0"$val".bz2
    echo "Bzip2 done"
done

python3 compare.py