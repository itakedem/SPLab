#include <stdio.h>


int main(int argc, char **argv)
{
    char c;
    while ((c = fgetc(stdin)) != EOF)
    {
        do
        {
            fprintf(out, "%d ", c);
        } while ((c = fgetc(in)) != '\n');

        fprintf(stdout, "\n");
    }

    return 0;
}