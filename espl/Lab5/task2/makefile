
%.o: %.asm
	nasm -g -O1 -f elf -o $@ $<

all: task2a task2c

task2a: task2a.o
	ld -melf_i386 -o task2a task2a.o

task2c: task2cLetterByLetter.o cmpstr.o utoa_s.o
	ld -melf_i386 -o task2cLetterByLetter task2cLetterByLetter.o

clean:
	rm -f *.o 
