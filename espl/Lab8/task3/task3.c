#include<stdio.h>
#include<stdlib.h>

// size of fib: 159

int fib(int limit) {
    int a0 = 1;
    int a1 = 1;
//    int a2[] = { 1, 1 };
//    char a3 = 'a';
//    char a4 = 'a';
//     char a5 = 'a';
//     char a6 = 'a';

    for (int i = 0; a0 + a1 < limit; i++) {
        a1 += a0;
        a0 = a1 - a0;
    }
    return a1;
}

int main(int argc, char **argv) {
    int limit = atoi(argv[1]);
    fib(limit);
    return 0;
}