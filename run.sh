#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: ./run.sh <day_number>"
    exit 1
fi

day=$1
day_dir="day${day}"
src_file="${day_dir}/main.cpp"
exec_file="${day_dir}/main.exe"

if [ ! -f "$src_file" ]; then
    echo "Error: Source file $src_file does not exist"
    exit 1
fi

# Compile and check if successful
if g++ -Wall -Wextra -Werror -Wuninitialized -pedantic -std=c++17 -O2 -o "$exec_file" "$src_file"; then
    # Save current directory
    current_dir=$(pwd)

    # Change to day directory and run
    cd "$day_dir" || exit 1
    ./main.exe

    # Return to original directory
    cd "$current_dir" || exit 1
else
    echo "Compilation failed"
    exit 1
fi