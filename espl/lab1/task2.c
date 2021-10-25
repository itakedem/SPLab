#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void char2ascii(int c, FILE* in, FILE* out);
void char2Binary(int c, FILE* in, FILE* out);
void lowHighSwitch(int c, FILE* in, FILE* out);
void resolver(int argc, char* task, FILE** out, FILE**in, char ** argv);


int main(int argc, char **argv)
{
    FILE *in = stdin;
    FILE *out = stdout;
    int c;

    char task = 'a';

    resolver(argc, &task, &out, &in, argv);

    while ((c = fgetc(in)) != EOF)
    {
        if (task == 'a')
            char2ascii(c, in, out);
        if (task == 'b')
            char2Binary(c, in, out);
        if (task == 'c')
            lowHighSwitch(c, in, out);
            fprintf(out, "\n");
    }
    fclose(in);
    fclose(out);

    return 0;
}

void resolver(int argc, char* task, FILE** out, FILE**in, char ** argv)
{
    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-i") == 0)
            *in = fopen(argv[++i], "r");
        
        if (strcmp(argv[i], "-o") == 0)
            *out = fopen(argv[++i], "w");
        
        if (strcmp(argv[i], "-b") == 0)
            *task = 'b';
        
        if (strcmp(argv[i], "-c") == 0)
            *task = 'c';
        
        i++;
    }
}
