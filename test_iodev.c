#include "iodev.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TERM_STR_LEN 80

static char dev[TERM_STR_LEN] = "/dev/";

static int prepare(char *test) {
  int df = open(dev, O_RDWR);
  if (df < 0)
    printf("open device error: %m\n");

  int len = strlen(test);
  int res = write(df, test, len);
  if (res != len)
    printf("write device error: %m\n");
  else
    printf("prepared %d bytes: %s\n", res, test);
  return df;
}

static void test(int df, int size) {
  char buf[IODEV_BUF_LEN + 1];
  int res;
  printf("------------------------------------\n");
  do {
    res = read(df, buf, size);
    if (res > 0) {
      buf[res] = '\0';
      printf("read %d bytes: %s\n", res, buf);
    } else if (res < 0)
      printf("read device error: %m\n");
    else
      printf("read end of stream\n");
  } while (res > 0);
  printf("------------------------------------\n");
}

int main(int argc, char *argv[]) {
  strcat(dev, IODEV_NAME);
  int df1 = prepare("1111111");
  int df2 = prepare("22222");
  test(df1, 10);
  test(df2, 30000);
  close(df1);
  close(df2);
  return EXIT_SUCCESS;
};
