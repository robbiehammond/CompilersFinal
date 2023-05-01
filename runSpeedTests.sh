./build.sh
cd bin
./LLVM-Lab -i ../tests/useless_while.c -fAsm -o ../tests/useless_while.ll
./LLVM-Lab -i ../tests/infinite_while.c -fAsm -o ../tests/infinite_while.ll
./LLVM-Lab -i ../tests/useless_for.c -fAsm -o ../tests/useless_for.ll
./LLVM-Lab -i ../tests/infinite_for.c -fAsm -o ../tests/infinite_for.ll
./LLVM-Lab -i ../tests/if_remove_then.c -fAsm -o ../tests/if_remove_then.ll
./LLVM-Lab -i ../tests/if_remove_else.c -fAsm -o ../tests/if_remove_else.ll
time lli ../tests/useless_while.ll
time lli ../tests/useless_for.ll
time lli ../tests/if_remove_else.ll
time lli ../tests/if_remove_then.ll

