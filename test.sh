if diff ./tests/add.l1.exe.asm ./tests/add.l1.expected-asm; then
    echo "passed add"
else
    echo "failed add"
fi

if diff ./tests/and.l1.exe.asm ./tests/and.l1.expected-asm; then
    echo "passed and"
else
    echo "failed and"
fi
