all:
	cd src; g++ Elf.cpp Assembling.cpp Token.cpp TokenList.cpp Tokenization.cpp Translation.cpp enums2string.cpp foperations.cpp main.cpp -o ../translate;

stdlib:
	cd src; nasm -f bin stdlib/out.asm; nasm -f bin stdlib/in.asm; nasm -f bin stdlib/hlt.asm;

clean:
	rm -f translate result