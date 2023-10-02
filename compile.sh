#!/bin/bash

function find_c_files {
    for file in "$1"/*; do
        if [[ -d "$file" ]]; then
            find_c_files "$file"
        elif [[ "$file" == *.c ]]; then
            c_files+=("$file")
        fi
    done
}

c_files=()
find_c_files "."

for file in "${c_files[@]}"; do
    echo "Compiling $file"
    gcc -o "${file%.c}" "$file"
done
