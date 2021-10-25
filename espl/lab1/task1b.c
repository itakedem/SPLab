#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void num2Binary(int n, int arr[8]);

void char2Binary(int c, FILE* in, FILE* out)
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

