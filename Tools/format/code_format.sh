#!/bin/bash

dirs=(
    ../../app/Inc
    ../../app/Src
    ../../hal/Inc
    ../../hal/Src
    ../../port/pc_linux
    ../../utl/Inc
    ../../utl/Src
)

for dir in "${dirs[@]}"; do
    echo "Formatting files in $dir"
    find "$dir" -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -style=file -i {} +
done

# arquivo isolado
clang-format -style=file -i ../../main.c