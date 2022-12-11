#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int fd = open("received1.dat", O_RDONLY, 0644);

  int n;
  int buf[10];
  while ((n = read(fd, buf, sizeof(int) * 10)) > 0)
  {
    int i;
    for (i = 0; i < n / 4; i++)
    {
      printf(" %d", buf[i]);
    }
    printf("\n");
  }
}