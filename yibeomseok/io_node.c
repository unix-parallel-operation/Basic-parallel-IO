/**
 * created by yibeomseok
 */

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define KEY_NUM_1 103861
#define KEY_NUM_2 103862
#define MTYPE 1

struct msgbuf
{
  long mtype;
  int mdata[256];
};

/* 어떤 message queue에서 receive할 것인지 인자로 줘야 함 */
int main(int argc, char *argv[])
{
  printf("[io_node start]\n");
  if (argc < 2)
  {
    perror("io_node");
    exit(1);
  }

  int msgq_id = atoi(argv[1]);
  int k_size = atoi(argv[2]);
  char *recv_file_name = argv[3];
  int fd, len;
  struct msgbuf msg;
  msg.mtype = MTYPE;
  sigset_t set;

  if ((fd = open(recv_file_name, O_CREAT | O_RDWR, 0644)) == -1)
  {
    perror("open");
    exit(1);
  }

  int i;
  for (i = 0; i < k_size * 1024 / 2; i++)
  {
    int temp[1] = {0};
    write(fd, temp, sizeof(int));
  }

  int w_cnt = 0;
  int last_cnt = k_size * 2;
  while (1)
  {
    len = msgrcv(msgq_id, &msg, 256 * sizeof(int), 1, 0);
    if (len == 0)
      continue;
    io_write(fd, msg.mdata, len);
    w_cnt++;
    printf("io_node count = %d\n", w_cnt);
    if (w_cnt == last_cnt)
    {
      msgctl(msgq_id, IPC_RMID, NULL);
      break;
    }
  }

  close(fd);
  printf("[io_node end]\n");
  return 0;
}

// int io_write(int fd, int *data, int byte_size)
// {
//   printf("io에서 256개 받고 쓰기 시작할 때 첫 정수 = %d\n\n", data[0]);
//   int d_cnt = byte_size / 4;
//   int i;
//   int buf[1];
//   for (i = 0; i < d_cnt; i++)
//   {
//     buf[0] = data[i];
//     lseek(fd, data[i] * 4, SEEK_SET);
//     write(fd, buf, sizeof(int));
//   }
// }

int io_write(int fd, int *data, int byte_size)
{
  int d_cnt = byte_size / 4;
  int i;
  int buf[1];
  int unit = data[0] - (data[0] % 1024);
  int sub = (unit / 1024);
  int st;
  if (sub == 0)
    st = data[0];
  else
    st = (unit / sub * (sub / 2)) + (data[0] % 4);
  off_t start = lseek(fd, st * 4, SEEK_SET);
  printf("io에서 256개 받고 쓰기 시작할 때 첫 정수 = %d\nstart = %d\n", data[0], (int)st);
  printf("io에 offset = %d위치에 쓰며 될것임\n\n", (int)start);
  
  for (i = 0; i < d_cnt; i++)
  {
    buf[0] = data[i];
    write(fd, buf, sizeof(int));
    start = lseek(fd, 3 * sizeof(int), SEEK_CUR);
  }
}