#!/bin/zsh

for i in tests/*.l1
do
    echo "Compiling $i to $i.exe.asm"
    ./build/c1 $i --gen-asm-only $i.exe.asm && (
        echo "Assembling and linking $i.asm with bootstrap code to produce $i.exe"
        ./build/c1 $i $i.exe
    )
done
