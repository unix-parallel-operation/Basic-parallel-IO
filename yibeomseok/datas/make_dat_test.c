#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  int data_fd;
  int buf[10];
  int n;
  int i;
  int count = 0;
  data_fd = open("data3.dat", O_RDONLY, 0644);

  while((n = read(data_fd, buf, 40)) > 0 ) {
    for(i = 0; i < n/4; i++) {
      printf("%d ", buf[i]);
    }
    printf("\n");
    count += n/4;
  }
  printf("개수 = %d\n", count);
  close(data_fd);
}