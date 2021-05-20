all:
	cd src; g++ main.cpp Token.cpp TokenList.cpp Tokenization.cpp Translation.cpp enums2string.cpp tests.cpp translator.cpp foperations.cpp -o ../translate;

stdlib:
	cd src; nasm -f elf64 stdlib/out.asm -o stdlib/out.o; nasm -f elf64 stdlib/in.asm -o stdlib/in.o; nasm -f elf64 stdlib/hlt.asm -o stdlib/hlt.o;
	
iotest:
	cd src; nasm -f elf64 stdlib/iotest.asm -o stdlib/iotest.o; ld -o ../iotest stdlib/in.o stdlib/out.o stdlib/iotest.o stdlib/hlt.o;

result:
	nasm -f elf64 result.asm -o result.o; cd src; ld -o ../result stdlib/in.o stdlib/out.o ../result.o stdlib/hlt.o;

clean:
	rm -f iotest translate result