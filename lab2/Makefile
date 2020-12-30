FELF=-felf64
NASM=nasm

all: main

main: main.o dict.o lib.o
	ld -o main main.o dict.o lib.o

main.o: main.asm colon.inc words.inc
	$(NASM) $(FELF) main.asm

lib.o: lib.asm 
	$(NASM) $(FELF) lib.asm

dict.o: dict.asm 
	$(NASM) $(FELF) dict.asm


clean:
	rm -f main.o lib.o dict.o main
