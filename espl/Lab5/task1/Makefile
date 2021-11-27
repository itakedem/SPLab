%.o: %.asm
	nasm -g -O1 -f elf -o $@ $<

%.o: %.c
	gcc -m32 -g -nostdlib -fno-stack-protector -c -o $@ $<

all: lwca

lwca: lwc.o start.o utoa_s.o
	ld  -melf_i386 -o $@ $^

task1c: task1c.o atou_s.o
	ld  -melf_i386 -o $@ $^


clean:
	rm -f *.o lwca task1c
