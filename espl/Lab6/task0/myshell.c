#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>

#include "LineParser.h"

char currDir[PATH_MAX];
char userText[2048];

int main(int argc, char** argv)
{
    getcwd(currDir, sizeof(currDir));
    while (1)
    {
        printf("%s$ ", currDir);
        fgets(userText, 2048 ,stdin);
        cmdLine* currLine = parseCmdLines(userText);
        if (strcmp(currLine->arguments[0], "quit") == 0)
        {
            freeCmdLines(currLine);
            printf("end of loop\n");
            break;
        }
        execute(currLine);
        printf("\n");
        freeCmdLines(currLine);
    }

    return 0;
}

void execute(cmdLine* currLine)
{
    int ans = execvp(currLine->arguments[0], currLine->arguments);
    if (ans == -1)
        perror("There was an error");
}