#include <stdio.h>

extern int open_file(const char* path, int mode);
extern int close_file(int desc);

int main(int argc, char const *argv[])
{
    if (argc != 2)
        return 1;

    int file_descriptor = open_file(argv[1], 0);
    printf("the file descriptor is %d\n", file_descriptor);

    int status;
    if ((status = close_file(file_descriptor)) >= 0)
        printf("CLOSED FILE\n");
    else
        printf("ERROR [%d]", status);

    return 0;
}