nasm -f elf64 branch_exp.asm -o branch_exp.o
gcc -no-pie branch_test.c branch_exp.o -o branch_test
rm branch_exp.o
./branch_test
