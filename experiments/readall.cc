#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *readall(const char *path) {

  struct stat statbuf;
  char *data;
  off_t pos;

  int fd = open(path, O_RDONLY);

  if (fd < 0)
    goto open_failed;

  if (fstat(fd, &statbuf) != 0)
    goto stat_failed;

  data = (char *)malloc(statbuf.st_size + 1);
  if (!data)
    goto malloc_failed;

  pos = 0;
  while (pos < statbuf.st_size) {
    int r = read(fd, data + pos, statbuf.st_size - pos);
    if (r < 0)
      goto read_failed;
    pos += r;
  }

  data[statbuf.st_size] = 0; // null-terminator

  return data;

read_failed:
  free(data);
malloc_failed:
stat_failed:
  close(fd);
open_failed:
  return NULL;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: readall PATH\n");
    return 1;
  }

  char *data = readall(argv[1]);
  if (!data) {
    printf("Error reading data\n");
    return 2;
  }
  printf("Data read from file '%s':\n", argv[1]);
  printf("%s\n", data);
}
