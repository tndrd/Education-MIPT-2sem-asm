MyPrint:
	g++ -c MyPrintTest.c -o MyPrintTest.o
	nasm -f elf64 MyPrint.asm -o MyPrint.o
	g++ -no-pie -o MyPrint MyPrintTest.o MyPrint.o 

StdPrintf:
	nasm -f elf64 StdPrintfCall.asm -o StdPrintf.o
	g++ -no-pie -o StdPrintf StdPrintf.o

clean:
	rm -f MyPrint.o MyPrintTest.o MyPrint StdPrintf.o StdPrintf 