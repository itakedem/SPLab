#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/wait.h>

#include "LineParser.h"

int execute(cmdLine* currLine);
int commands(cmdLine* line);
int executeCD(cmdLine* line);
int executeHis();


char currDir[PATH_MAX];
char userText[2048];
cmdLine* history[2048];
int endOfHistory;

int main(int argc, char** argv)
{
    endOfHistory = 0;
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
        history[endOfHistory++] = currLine;
        commands(currLine);
        printf("\n");
    }

    return 0;
}

int commands(cmdLine* line)
{
    char* command = line->arguments[0];
    if (strcmp("cd", command) == 0)
        return executeCD(line);
    if (strcmp("history", command) == 0)
        return executeHis();
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
    {
        return chdir(getenv("HOME"));
    }
    return chdir(strcat(strcat(currDir, "/"), path));
}

int executeHis()
{
    int i = 0;
    while (i < endOfHistory)
    {
        int j = 0;
        while (j < history[i]->argCount)
        {
            printf("%s ", history[i]->arguments[j]);
            j++;
        }
        printf("\n");
        i++;
    }
    return 0;
}

/* TODO: add a recursive free cmdLine */