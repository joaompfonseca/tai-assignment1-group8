#!/bin/bash

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

# Compress the main file
tar -cJf example/chry.tar.xz example/chry.txt
echo "Tar done"
gzip -c example/chry.txt > example/chry.gz
echo "Gzip done"
bzip2 -c example/chry.txt > example/chry.bz2
echo "Bzip2 done"

python3 compare.py