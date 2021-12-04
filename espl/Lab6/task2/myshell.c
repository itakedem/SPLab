#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "LineParser.h"

int execute(cmdLine* currLine);
int commands(cmdLine* line);
int executeCD(cmdLine* line);
int executeHis();
void freeHistory();
int executeReuse(char* ind);

char currDir[PATH_MAX];
char userText[2048];
cmdLine* history[10000000];
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
            freeHistory();
            freeCmdLines(currLine);
            printf("end of loop\n");
            break;
        }
        int ans = commands(currLine);
        if (ans == -1)
            perror("There was an error");
        printf("\n");
    }

    return 0;
}

int commands(cmdLine* line)
{
    char* command = line->arguments[0];
    if (strcmp("cd", command) == 0)
    {
        history[endOfHistory++] = line;
        return executeCD(line);
    }
    if (strcmp("history", command) == 0)
    {
        history[endOfHistory++] = line;
        return executeHis();
    }
    if (command[0] == '!')
    {
        int success =  executeReuse(&command[1]);
        freeCmdLines(line);
        return success;
    }
    history[endOfHistory++] = line;
    return execute(line);
}

int executeReuse(char* ind)
{
    int index = atoi(ind);
    if (index >= endOfHistory)
    {
        fprintf(stderr, "Index is out of history list's bounds\n");
        return -2;
    }
    
    cmdLine* line = clone(history[index]);
    
    return commands(line);
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
        }
        _exit(ans);
    }
    else
    {
        if (line->blocking)
            waitpid(pid, &status, 0);
    }
    return 0;
}

void freeHistory()
{
    int i=0;
    while (i < endOfHistory)
    {
        if (history[i] != NULL)
        {
            freeCmdLines(history[i]);
        }
        i++;
    }
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