#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main (int argc, char** argv){
    int iarray[3];
    char carray[3];
    printf("- &iarray: %p\n",&(iarray[0]));
    printf("- &iarray: %p\n",&(iarray[1]));
    printf("- &iarray: %p\n",&(iarray[2]));

    printf("- &iarray: %p\n",&(carray[0]));
    printf("- &iarray: %p\n",&(carray[1]));
    printf("- &iarray: %p\n",&(carray[2]));

    printf("- &iarray: %p\n",iarray);
    printf("- &iarray+1: %p\n",iarray+1);
    printf("- &carray: %p\n",carray);
    printf("- &carray+1: %p\n",carray+1);
    return 0;
}