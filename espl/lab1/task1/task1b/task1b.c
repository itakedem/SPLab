#include <stdio.h>


void num2Binary(int n, int arr[8]);
void char2Binary(int c, FILE* in, FILE* out);
void resolver(int argc, char* task, FILE** out, FILE**in, char ** argv);
void char2ascii(int c, FILE* in, FILE* out);



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
        fprintf(out, "\n");
    }
    fclose(in);
    fclose(out);

    return 0;
}

void char2ascii(int c, FILE* in, FILE* out)
{

        fprintf(out, "%d ", c);
}

void char2Binary(int c, FILE* in, FILE* out)
{
	int arr[8];
	int i;

		num2Binary(c, arr);
		for (i = 0; i < 8; i++)
			fprintf(out, "%d", arr[i]);
		fprintf(out, " ");
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

void resolver(int argc, char* task, FILE** out, FILE**in, char ** argv)
{
    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-b") == 0)
            *task = 'b';
        i++;
    }
}