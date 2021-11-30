#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    char currDir[PATH_MAX];
    getcwd(currDir, sizeof(currDir));
    printf(currDir);
}