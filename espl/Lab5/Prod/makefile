
%.o: %.asm
	nasm -g -O1 -f elf -o $@ $<

task2: task2a task2c

task1: lwca task1c

task2a: task2a.o
	ld -melf_i386 -o task2a task2a.o

task2c: task2c.o cmpstr.o utoa_s.o
	ld -melf_i386 -o task2c task2c.o

lwca: lwc.o start.o utoa_s.o
	ld  -melf_i386 -o $@ $^

task1c: task1c.o start.o utoa_s.o atou_s.o
	ld  -melf_i386 -o $@ $^

clean:
	rm -f *.o 
