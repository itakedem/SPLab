#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>

int main(int argc, char** argv)
{
    char currDir[PATH_MAX];
    getcwd(currDir, sizeof(currDir));
    printf(currDir);
}