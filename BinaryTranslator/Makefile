all:
	cd src; g++ main.cpp Token.cpp TokenList.cpp enums2string.cpp tests.cpp translator.cpp ../../../Education-MIPT/Onegin/foperations.cpp -o ../translate;

stdlib:
	cd src; nasm -f elf64 stdlib/out.asm -o stdlib/out.o; nasm -f elf64 stdlib/in.asm  -o stdlib/in.o;
	
iotest:
	cd src; nasm -f elf64 stdlib/iotest.asm -o stdlib/iotest.o; nasm -f elf64 stdlib/in.asm -o stdlib/in.o; ld -o ../iotest stdlib/in.o stdlib/iotest.o;

clean:
	rm -f iotest translate