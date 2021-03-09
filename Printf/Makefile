all:
	g++ -c CommonCProgram.c -o CCP.o
	nasm -f elf64 MyPrint.asm -o MyPrint.o
	g++ -no-pie -o MyPrint CCP.o MyPrint.o 