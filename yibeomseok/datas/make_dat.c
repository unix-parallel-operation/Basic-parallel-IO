#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char *data_names[] = {
    "data0.dat",
    "data1.dat",
    "data2.dat",
    "data3.dat"};

int openData(int data_number)
{
  int fd;

  fd = open(data_names[data_number], O_CREAT|O_WRONLY, 0644);
  if (fd == -1)
  {
    perror("open");
    exit(1);
  }

  return fd;
}

void fillData(int data_fd, int data_number, int k_size)
{
  int start_number = data_number;
  int last_number = k_size * 1024 - 4 + data_number;
  int i;
  int buf[k_size * 1024 / 4]; // 총 노드 개수가 4개라서 데이터 개수가 4개이고 그래서 4분할

  for (i = 0; i < k_size * 1024 / 4; i++)
  {
    buf[i] = start_number + (i * 4);
  }

  if (write(data_fd, buf, k_size * 1024/4 * sizeof(int)) != k_size * 1024/4 * sizeof(int))
  {
    perror("write");
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int data_0, data_1, data_2, data_3;
  int k_size;

  if (argc < 2)
  {
    printf("type: 16, 64\n");
    exit(1);
  }

  k_size = atoi(argv[1]);

  data_0 = openData(0);
  fillData(data_0, 0, k_size);

  data_1 = openData(1);
  fillData(data_1, 1, k_size);

  data_2 = openData(2);
  fillData(data_2, 2, k_size);

  data_3 = openData(3);
  fillData(data_3, 3, k_size);

  close(data_0);
  close(data_1);
  close(data_2);
  close(data_3);
}
