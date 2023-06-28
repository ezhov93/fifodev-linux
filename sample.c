#include "iodev.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TERM_STR_LEN 80

int main(int argc, char *argv[]) {
  char dev[TERM_STR_LEN] = "/dev/";
  strcat(dev, IODEV_NAME);
  int df = open(dev, O_RDWR);
  if (df < 0)
    printf("open device error: %m\n"), exit(EXIT_FAILURE);
  char msg[160];
  fprintf(stdout, "> ");
  fflush(stdout);
  scanf("%s", msg);
  int len = strlen(msg);
  int res = write(df, msg, len);
  if (res != len)
    printf("write device error: %m\n");
  char *p = msg;
  do {
    res = read(df, p, sizeof(msg));
    if (res > 0) {
      *(p + res) = '\0';
      printf("read %d bytes: %s\n", res, p);
      p += res;
    } else if (res < 0)
      printf("read device error: %m\n");
  } while (res > 0);
  fprintf(stdout, "%s\n", msg);
  close(df);
  return EXIT_SUCCESS;
};
