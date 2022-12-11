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

struct msgbuf
{
  long mtype;
  int mdata[256];
};

void handler(int signo) {
}

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
  msg.mtype = 1;
  sigset_t set;

  if ((fd = open(recv_file_name, O_CREAT | O_RDWR, 0644)) == -1)
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

  int w_cnt = 0;
  int last_cnt = k_size * 2;
  while (1)
  {
    len = msgrcv(msgq_id, &msg, 256 * sizeof(int), 1, 0);
    if(len == 0) continue;
    io_write(fd, msg.mdata, len);
    w_cnt++;
    printf("io_node count = %d\n", w_cnt);
    if(w_cnt == last_cnt){
      msgctl(msgq_id, IPC_RMID, NULL);
      break;
    }
  }

  close(fd);
  printf("[io_node end]\n");
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
    lseek(fd, data[i] * 4, SEEK_SET);
    write(fd, buf, sizeof(int));
  }
}