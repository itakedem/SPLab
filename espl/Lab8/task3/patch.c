#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

// params: ./patch fib 1325 66 fibonacci 1613
int main(int argc, char **argv) {
    char *source_file = argv[1];
    int source_pos = atoi(argv[2]);
    size_t size = atoi(argv[3]);
    char *target_file = argv[4];
    int target_pos = atoi(argv[5]);

    int source_fd = open(source_file, O_RDONLY);
    struct stat source_st;
    fstat(source_fd, &source_st);
    size_t source_size = source_st.st_size;

    int target_fd = open(target_file, O_RDWR);
    struct stat target_st;
    fstat(target_fd, &target_st);
    size_t target_size = target_st.st_size;

    char *source_content = mmap(NULL, source_size, PROT_READ, MAP_PRIVATE, source_fd, 0);
    char *target_content = mmap(NULL, target_size, PROT_READ | PROT_WRITE, MAP_SHARED, target_fd, 0);

    memcpy(target_content + target_pos, source_content + source_pos, size);

    close(source_fd);
    close(target_fd);

    return 0;
}