#include <stdio.h>


int main(int argc, char **argv)
{
    char c;
    while ((c = fgetc(stdin)) != EOF)
    {
        fprintf(out, "%d ", c);
        fprintf(stdout, "\n");
    }

    return 0;
}