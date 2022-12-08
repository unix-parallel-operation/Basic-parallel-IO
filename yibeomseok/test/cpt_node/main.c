/**
 * Basic parallel I/O
 * 각 compute node가 주어진 data들을 개별로 I/O node에 전송하는 방법
 *
 * ex)
 * compute node #0가 D(0)와 D(16)은 I/O node #0로 보내고 D(8)과 D(24)는 I/O node #1로 전송한다.
 * 각 I/O node는 받은 data들을 하나씩 저장한다.
 * 
 * created by yibeomseok
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/computenode.h"
#include <string.h>

#define KEY_NUM_1 103861
#define KEY_NUM_2 103862

struct msgbuf
{
  long mtype;
  int mdata[256];
};

void set_argv(char *argv[], int data_num, int msgq_id[], char* k_size);

int main(int argc, char* argv[])
{
  if(argc < 2) {
    /* 노드 사이즈 입력일 뿐 데이터 사이즈는 변하지 않는다. */
    perror("노드 사이즈를 입력해야 함 16 or 64");
    exit(1);
  }

  int pid;
  int msgq[2];
  char* k_size = argv[1];
  msgq[0] = msgget(KEY_NUM_1, IPC_CREAT | 0644);
  msgq[1] = msgget(KEY_NUM_2, IPC_CREAT | 0644);
  char *execv_argv[6];


  // 이제 메시지 큐로 받을 수 있는지 fork & exec해서 확인해보도록 한다.
  pid = fork();
  if (pid == 0)
  {
    set_argv(execv_argv, 0, msgq, k_size);
    if (execv("/home/beomseok/projects/Basic-parallel-IO/yibeomseok/test/cpt_node/compute_node", execv_argv) == -1)
    {
      perror("execv");
      exit(1);
    }
  }

  return 0;
}

void set_argv(char *argv[], int data_num, int msgq_id[], char* k_size) /* k_size == 16 or 64*/
{
  char path[20];
  char s_msgq_id_0[20], s_msgq_id_1[20];
  sprintf(path, "data%d.dat", data_num);
  sprintf(s_msgq_id_0, "%d", msgq_id[0]);
  sprintf(s_msgq_id_1, "%d", msgq_id[1]);
  
  argv[0] = "compute_node";
  argv[1] = path;
  argv[2] = s_msgq_id_0;
  argv[3] = s_msgq_id_1;
  argv[4] = k_size; // 변경해줘야 함
  argv[5] = NULL;
}