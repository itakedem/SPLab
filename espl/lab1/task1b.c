#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void char2Binary(int c, FILE* in, FILE* out)
{
	int ans[8];
	int i;
	do
	{
		num2Binary(c, ans);
		for (i = 0; i < 8; i++)
			fprintf(out, "%d", ans[i]);
		fprintf(out, " ");
		
	} while ((c = fgetc(in)) != '\n');
}


void num2Binary(char c, int arr[8])
{
	int val = c - 0;
	int i = 7;
	while (i >= 0)
	{
		arr[i] = val & 1;
		val >>= 1;
		i--;
	}

}

