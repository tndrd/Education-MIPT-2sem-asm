all:
	g++ -c MyPrintTest.c -o MyPrintTest.o
	nasm -f elf64 MyPrint.asm -o MyPrint.o
	g++ -no-pie -o MyPrint MyPrintTest.o MyPrint.o 