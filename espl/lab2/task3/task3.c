#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BUFFERSIZE 10

struct fun_desc {
    char *name;

    void (*fun)(char);
};

FILE * in;
FILE * out;


void echo_printer(char c); /* task 0 from lab1 - prints c to the standard output */
void ascii_printer(char c); /* task 1a from lab1 - print the ASCII code of c to the standard output*/
void binary_printer(char c); /* task 1b from lab1 – print the binary representation of c to the standard output */
void lower_to_upper_printer(char c); /* task 1c from lab1 – print c to the standard output in upper case */
void arr_printer(int* arr, int len);
void operate(struct fun_desc menu[4]);
void or(int sum[8], int carry[8]);

void echo_printer(char c)
{
    fprintf(stdout, "%c", c);
}
void ascii_printer(char c)
{
    fprintf(stdout, "%d ", c);
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

void binary_printer(char c)
{
    int arr[8];
    num2Binary(c, arr);
    arr_printer(arr, 8);
}

void lower_to_upper_printer(char c)
{
    if(islower(c))
    {
        fprintf(out, "%c", toupper(c));
    }
    else if(isupper(c))
    {
        fprintf(out, "%c", tolower(c));
    }
    else
    {
        fprintf(out,"%c", c);
    }
}

void string_printer(char* str, void (*func_ptr) (char))
{
    char c;
    int i = 0;
    while ((c = str[i]) != '\n')
    {
        func_ptr(c);
        i++;
    }
    fprintf(out, "\n");
}

void string_reader(char* s)
{
    fgets(s, BUFFERSIZE, in);
}

void bitwise_or(char* s)
{
    int carry[8];
    int sum[8] = {0,0,0,0,0,0,0,0};
    int i = 0;
    char c;
    while ((c = s[i]) != '\n')
    {
        num2Binary(c, carry);
        or(sum, carry);
        i++;
    }
    arr_printer(sum, 8);
    fprintf(out, "\n");
}

void arr_printer(int* arr, int len)
{
    int i = 0;
    while (i < len)
    {
        fprintf(out, "%d", arr[i]);
        i++;
    }
    fprintf(out, " ");
}

void or(int sum[8], int carry[8])
{
    int i = 0;
    while (i < 8)
    {
        if (!(sum[i] == 0 && carry[i] == 0))
            sum[i] = 1;
        i++;
    }
}

int main(int argc, char **argv)
{
    in = stdin;
    out = stdout;
    struct fun_desc menu[4] = {
            {"echo printer", echo_printer},
            {"ascii printer", ascii_printer},
            {"binary printer", binary_printer},
            {"lower to upper printer", lower_to_upper_printer}};
    operate(menu);
    fclose(in);
    fclose(out);
    return 0;
}

void operate(struct fun_desc menu[4])
{
    char str[10];
    fprintf(out, "%s", "Please enter a string (0<size<=10): \n");
    string_reader(&str[0]);
    int option;
    int eof;
    fprintf(out, "%s", "Please choose printer type: \n");
        int i = 0;
        while (i < 4)
        {
            fprintf(out, "%d) %s", i, menu[i].name);
            fprintf(out, "\n");
            i++;
        }
        fprintf(out, "%s", "5) bitwise or");
        fprintf(out, "\n\n");
    do
    {
        fprintf(out, "Option: ");
        eof = scanf("%d",&option);
        if(eof <= 0)
        {
            fprintf(out, "DONE.\n");
            break;
        }
        if (option > 5 || option < 0)
        {
            fprintf(out, "%s", "Please select 1-4 or CTRL-D to exit. \n");
            continue;
        }
        else if (option == 4)
            bitwise_or(str);
        else
            string_printer(str, menu[option].fun);
    } while (1);

}






