#include <stdio.h>

void utoa_s(int num, char* buf);
int atoa_s(char* str);
int write(int fd, char *buf, int size);
int strlen(char *s);
void print(char *s) {
	write(1, s, strlen(s));
}

int main(int argc, char **argv){
	char buffer[11]; 
	utoa_s(0, buffer); 
	print(buffer); print("\n");

	int i = atoa_s("123");
	i++;

	char buffer2[11]; 
	utoa_s(i, buffer2); 
	print(buffer2); print("\n");

	return 0;
}

