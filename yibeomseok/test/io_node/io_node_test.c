/**
 * created by yibeomseok
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/msg.h>

#define KEY_NUM_1 103861
#define KEY_NUM_2 103862

struct msgbuf
{
  long mtype;
  int mdata[256];
};

int main(int argc, char *argv[])
{
  int msgq_id = atoi(argv[1]);
  int k_size = atoi(argv[2]);
  int fd, len;
  struct msgbuf msg;

  if ((fd = open("received.dat", O_CREAT | O_RDWR, 0644)) == -1)
  {
    perror("open");
    exit(1);
  }
  int i;
  for (i = 0; i < k_size * 1024; i++)
  {
    int temp[1] = {0};
    write(fd, temp, sizeof(int));
  }

  while (1)
  {
    len = msgrcv(msgq_id, &msg, 256 * sizeof(int), 0, 0);
    io_write(fd, msg.mdata, len);
  }

  return 0;
}

int io_write(int fd, int *data, int byte_size)
{
  int d_cnt = byte_size / 4;
  int i;
  int buf[1];
  for (i = 0; i < d_cnt; i++)
  {
    buf[0] = data[i];
    lseek(fd, data[i], SEEK_SET);
    write(fd, buf, sizeof(int));
  }
}