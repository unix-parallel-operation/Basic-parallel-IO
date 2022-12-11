/**
 * created by yibeomseok
 *
 * Basic parallel I/O
 * 각 compute node가 주어진 data들을 개별로 I/O node에 전송하는 방법
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define KEY_NUM_1 103861
#define KEY_NUM_2 103862

void set_cpt_argv(char *argv[], int data_num, int msgq_id[], char *k_size); /* k_size == 16 or 64*/
void set_io_argv(char *argv[], char *msgq_id, char *k_size, char *recv_file_name);
void start_io_node(int *msgq, char* k_size);
void start_cpt_node(int *msgq, char *k_size);

// compute node size를 넣어주어야 하며 .dat 파일이 준비되어 있어야 함.
int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    perror("compute node size 16 or 64");
    exit(1);
  }

  /* ------------init------------ */
  int pid;
  int msgq[2] = {
      msgget(KEY_NUM_1, IPC_CREAT | 0644),
      msgget(KEY_NUM_2, IPC_CREAT | 0644)};
  char *k_size = argv[1];
  
  /* start io node */
  start_io_node(msgq, k_size);

  /* start compute node */
  start_cpt_node(msgq, k_size);

  return 0;
}

void set_cpt_argv(char *argv[], int data_num, int msgq_id[], char *k_size) /* k_size == 16 or 64*/
{
  char path[20];
  char s_msgq_id_0[20], s_msgq_id_1[20];
  sprintf(path, "./datas/data%d.dat", data_num);
  sprintf(s_msgq_id_0, "%d", msgq_id[0]);
  sprintf(s_msgq_id_1, "%d", msgq_id[1]);

  argv[0] = "compute_node";
  argv[1] = path;
  argv[2] = s_msgq_id_0;
  argv[3] = s_msgq_id_1;
  argv[4] = k_size; // 변경해줘야 함
  argv[5] = NULL;
}

void set_io_argv(char *argv[], char *msgq_id, char *k_size, char *recv_file_name)
{
  argv[0] = "io_node";
  argv[1] = msgq_id;
  argv[2] = k_size;
  argv[3] = recv_file_name;
  argv[4] = NULL;
}

void start_io_node(int *msgq, char* k_size)
{
  int pid;
  char msgqid[20];
  char *io_argv[5];
  char recv_file_name[30];
  int i;

  for (i = 0; i < 2; i++)
  {
    pid = fork();
    sprintf(msgqid, "%d", msgq[i]);
    sprintf(recv_file_name, "received%d.dat", i);
    set_io_argv(io_argv, msgqid, k_size, recv_file_name);
    if (pid == 0)
    {
      if (execv("./io_node", io_argv) == -1)
      {
        perror("execv: io node");
        exit(1);
      }
    }
  }
}

void start_cpt_node(int *msgq, char *k_size)
{
  int pid;
  int i;
  char *cpt_argv[6];
  for (i = 0; i < 4; i++)
  {
    pid = fork();
    if (pid == 0)
    {
      set_cpt_argv(cpt_argv, i, msgq, k_size);
      if (execv("./compute_node", cpt_argv) == -1)
      {
        perror("execv: compute node");
        exit(1);
      }
    }
  }
}