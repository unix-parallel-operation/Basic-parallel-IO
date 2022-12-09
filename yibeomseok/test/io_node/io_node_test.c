/**
 * created by yibeomseok
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

#define KEY_NUM_1 103861
#define KEY_NUM_2 103862

struct msgbuf
{
  long mtype;
  int mdata[256];
};

int main()
{
  struct msgbuf msg;
  int msgid[2];
  int keynum = 1;
  int len;
  msgid[0] = msgget(KEY_NUM_1, 0);
  msgid[1] = msgget(KEY_NUM_2, 0);
  
  len = msgrcv(msgid[keynum], &msg, 256 * sizeof(int), 0, 0);
  printf("Received Msg, len = %d\n", (int)len);
  int i;
  for(i = 0; i < len/4; i++) {
    printf("%d ", msg.mdata[i]);
  }
  printf("\n");
}