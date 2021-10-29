#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE * output=stdout;
    int size=0;
    for (int i = 1; i < argc; i++)
    {
        size+= strlen(argv[i]) +1;
    }
    
    char *str = (char *)malloc(size); 
    for(int i=1; i<argc; i++){
        strcat(str, argv[i]);
        strcat(str, " ");
    }
    printf(strcat(str, "\n"));
    return 0;
}
    
