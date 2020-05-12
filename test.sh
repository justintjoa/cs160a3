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

if diff ./tests/arith.l1.exe.asm ./tests/arith.l1.expected-asm; then
    echo "passed arith"
else
    echo "failed arith"
fi

if diff ./tests/eq.l1.exe.asm ./tests/eq.l1.expected-asm; then
    echo "passed eq"
else
    echo "failed eq"
fi


if diff ./tests/arith.l1.exe.asm ./tests/arith.l1.expected-asm; then
    echo "passed arith"
else
    echo "failed arith"
fi

if diff ./tests/int.l1.exe.asm ./tests/int.l1.expected-asm; then
    echo "passed int"
else
    echo "failed int"
fi

if diff ./tests/int2.l1.exe.asm ./tests/int2.l1.expected-asm; then
    echo "passed int2"
else
    echo "failed int2"
fi

if diff ./tests/le.l1.exe.asm ./tests/le.l1.expected-asm; then
    echo "passed le"
else
    echo "failed le"
fi

if diff ./tests/loop.l1.exe.asm ./tests/loop.l1.expected-asm; then
    echo "passed loop"
else
    echo "failed loop"
fi

if diff ./tests/lt.l1.exe.asm ./tests/lt.l1.expected-asm; then
    echo "passed lt"
else
    echo "failed lt"
fi

if diff ./tests/mul.l1.exe.asm ./tests/mul.l1.expected-asm; then
    echo "passed mul"
else
    echo "failed mul"
fi

if diff ./tests/neg.l1.exe.asm ./tests/neg.l1.expected-asm; then
    echo "passed neg"
else
    echo "failed neg"
fi

if diff ./tests/or.l1.exe.asm ./tests/or.l1.expected-asm; then
    echo "passed or"
else
    echo "failed or"
fi

if diff ./tests/recursion.l1.exe.asm ./tests/recursion.l1.expected-asm; then
    echo "passed recursion"
else
    echo "failed recursion"
fi

if diff ./tests/sub.l1.exe.asm ./tests/sub.l1.expected-asm; then
    echo "passed sub"
else
    echo "failed sub"
fi

if diff ./tests/var.l1.exe.asm ./tests/var.l1.expected-asm; then
    echo "passed var"
else
    echo "failed var"
fi

