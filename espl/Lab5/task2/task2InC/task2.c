#include <stdio.h>
#include <string.h>

FILE* fileName;
char task;
char* wordToCount;

void handleArg(int argc, char **argv){
    switch (argc)
    {
        case 2:
        {
            fileName = fopen(argv[1],"r");
            task = 'a';
            break;
        }
        case 3:
        {
            fileName = fopen(argv[2],"r");
            task = 'b';
            break;
        } 
        
        case 4:
        {
            fileName = fopen(argv[3],"r");
            wordToCount = argv[2];
            task = 'c';
            break;
        }
    }
}

void printFile(){
    char c;
    while ((c = fgetc(fileName)) != EOF)
    {
        printf("%c", c);
    }
    
}

void printNumOfWords(){
    char c;
    int numOfWords=1;
    while ((c = fgetc(fileName)) != EOF)
    {
        if(c == ' '){
            numOfWords++;
        }
    }
    printf("%d\n", numOfWords);
}

void printNumOfOccurences(){
    char c;
    int i=0;
    int len = strlen(wordToCount);
    int numOfWords=0;
    while ((c = fgetc(fileName)) != EOF)
    {
        while(i <len && wordToCount[i] == c){
            i++;
            if(((c = fgetc(fileName))== EOF || c == ' ' ) && i == len)
            {
                numOfWords++;
            }
        }
        i=0;
        
    }
    printf("%d\n", numOfWords);
}

int main(int argc, char **argv)
{
    handleArg(argc, argv);
    if (task == 'a'){
        printFile();
    }
    if (task == 'b'){
        printNumOfWords();
    }
    if (task == 'c'){
        printNumOfOccurences();
    }

    fclose(fileName);
    return 0;
}