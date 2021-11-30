#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/wait.h>

#include "LineParser.h"

int execute(cmdLine* currLine);
int commands(cmdLine* line);
int executeCD(cmdLine* line);


char currDir[PATH_MAX];
char userText[2048];

int main(int argc, char** argv)
{
    while (1)
    {
        getcwd(currDir, sizeof(currDir));
        printf("%s$ ", currDir);
        fgets(userText, 2048 ,stdin);
        cmdLine* currLine = parseCmdLines(userText);
        if (strcmp(currLine->arguments[0], "quit") == 0)
        {
            freeCmdLines(currLine);
            printf("end of loop\n");
            break;
        }
        commands(currLine);
        printf("\n");
        freeCmdLines(currLine);
    }

    return 0;
}

int commands(cmdLine* line)
{
    char* command = line->arguments[0];
    if (strcmp("cd", command) == 0)
        return executeCD(line);
    else
        return execute(line);

}

int execute(cmdLine* line)
{
    int status = 0;
    int pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        return 1;
    }
    if (pid == 0)
    {
        int ans = execvp(line->arguments[0], line->arguments);
        if (ans == -1)
        {
            perror("There was an error");
            _exit(1);
        }
    }
    waitpid(pid ,&status, 0);
    return 0;
}

int executeCD(cmdLine* line)
{
    char* path = line->arguments[1];
    if (path[0] == '~')
        return chdir(path);

    return chdir(strcat(strcat(currDir, "/"), path));
}