#include <stdio.h>
#include <string.h>

void char2ascii(int c);
void char2Binary(int c);
void lowHighSwitch(int c);
void resolver(int argc, char* task, char ** argv);


int main(int argc, char **argv)
{
    in = stdin;
    out = stdout;
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

void char2ascii(int c)
{
    do
    {
        fprintf(out, "%d ", c);
    } while ((c = fgetc(in)) != '\n');
}

void char2Binary(int c)
{
    int arr[8];
    int i;
    do
    {
        num2Binary(c, arr);
        for (i = 0; i < 8; i++)
            fprintf(out, "%d", arr[i]);
        fprintf(out, " ");

    } while ((c = fgetc(in)) != '\n');
}


void num2Binary(int n, int arr[8])
{
    int i = 7;
    while (i >= 0)
    {
        arr[i] = n & 1;
        n >>= 1;
        i--;
    }
}
