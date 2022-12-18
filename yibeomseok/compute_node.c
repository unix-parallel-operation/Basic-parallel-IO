/**
 * created by yibeomseok
 */

#include "./include/computenode.h"
#include <sys/msg.h>
#include <string.h>

struct msgbuf
{
  long mtype;
  int mdata[256];
};

/* 어떤 data를 읽는지와 msgq id 2개, k_size를 인자로 전달 */
int main(int argc, char *argv[])
{
  // 인자 제대로 전달 되었는지 확인용 포인터 쓰다가 전달 안 되는 경우가 있었음
  int i;
  for (i = 0; i <= 5; i++)
  {
    printf("[%s]\n", argv[i]);
  }

  /* -------------------------start------------------------- */
  if (argc < 4)
  {
    perror("argc < 4");
    exit(1);
  }

  /* --------------------------init-------------------------- */
  struct msgbuf msg;
  msg.mtype = 1;
  char *data_path = argv[1];
  int msgq[2] = {atoi(argv[2]), atoi(argv[3])};
  int k_size = atoi(argv[4]);
  struct cpt_node node = get_cpt_node(data_path, k_size, msgq);
  int n, next_msgq = 0;
  /* -------------------------------------------------------- */

  int sendingcount = 1;
  while ((n = read(node.data_fd, msg.mdata, 256 * sizeof(int))) > 0)
  {
    msgsnd(node.msgq_id[next_msgq], (void *)&msg, n, 0);
    next_msgq = (next_msgq + 1) % 2;
    printf("%s:: %d번째 전송\n", data_path, sendingcount++);
  } // fin sending
  printf("%s 끝:: %d %d\n",data_path, node.msgq_id[0], node.msgq_id[1]);
  
  close_cpt_node(node);
}