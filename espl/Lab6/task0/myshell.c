#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>

#include <LineParser.h>

char currDir[PATH_MAX];
char userText[2048];

int main(int argc, char** argv)
{
    getcwd(currDir, sizeof(currDir));
    while (1)
    {
        printf("%s$ ", currDir);
        fgets(userText, stdin);
        cmdLine currLine = parseSingleCmdLine(&userText);
        execute(currLine);
        printf("\n");
        if (cmpstr(currLine.arguments[0], "quit") == 0)
        {
            freeCmdLines(currLine);
            printf("end of loop");      /*TODO: remove */
            break;
        }
        freeCmdLines(currLine);
    }
}

void execute(cmdLine* currLine)
{
    int ans = execv(currDir, currLine->arguments);
    if (ans == -1)
        perror("There was an error");

}