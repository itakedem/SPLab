#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/types.h>
#include <libgen.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int calc_size(int fd)
{
    struct stat st;
    fstat(fd, &st);
    return st.st_size;
}

int patch(char* source_file, int source_pos, int size, char* dest_file, int dest_pos){
  int fdsrc = open(source_file, O_RDONLY, 0);
  int fdest = open(dest_file, O_RDWR, 0x0777);
      if (fdest == -1)
    {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }  
  char* srcFile = (char *)mmap(NULL, calc_size(fdsrc), PROT_READ, MAP_SHARED, fdsrc, 0);
  char* destFile = (char *)mmap(NULL, calc_size(fdest), PROT_READ | PROT_WRITE, MAP_SHARED, fdest, 0);
     if (destFile == MAP_FAILED)
    {
        close(fdest);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
  char* copy = (char*)malloc(size);
  memcpy(copy, srcFile + source_pos, size);

  for (int i = 0; i < size; i++)
    {
        int pos = i + dest_pos;
        
        destFile[pos] = copy[i];
    }
  msync(destFile, calc_size(fdsrc), MS_SYNC);
  msync(destFile, calc_size(fdest), MS_SYNC);
  close(fdsrc);
  close(fdest);
  free(copy);
  return 0;
}

int main(int argc, char** argv) {
  patch(argv[1], atoi(argv[2]), atoi(argv[3]), argv[4], atoi(argv[5]));
  return 0;
}