#include <stdio.h>

extern int funcA(char* str);

int main(int argc, char **argv)
{
  char * str = "Itamar";
  int len = funcA(str);
  printf("%d\n", len);
	
  return 0;
}