#include <stdio.h>


void char2ascii(int c, FILE* in, FILE* out) 
{
	do
	{
		fprintf(out, "%d ", c);
	} while ((c = fgetc(in)) != '\n');
}

int main(int argc, char **argv)
{
    char c;
    while ((c = fgetc(stdin)) != EOF)
    {
        char2ascii(c, stdin, stdout);
        fprintf(stdout, "\n");
    }

    return 0;
}