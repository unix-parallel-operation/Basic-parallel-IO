#include "./include/computenode.h"
#include <sys/msg.h>

 /* 어떤 data를 읽는지와 msgq id 2개, k_size를 인자로 전달 */ 
int main(int argc, char** argv){
  if(argc < 4) {
    perror("argc < 4");
    exit(1);
  }

  char* data_path = argv[1];
  int msgq[2] = { atoi(argv[2]), atoi(argv[2]) };
  int k_size = atoi(argv[4]);
  
  struct cpt_node node = get_cpt_node(data_path, k_size, msgq);

  int n, next_msgq = 0, buf[256];
  while((n = read(node.data_fd, buf, 256 * sizeof(int))) > 0) {
    msgsnd(node.msgq_id[next_msgq], buf, n, 0);
    next_msgq = (next_msgq + 1) % 2;
  }

  close_cpt_node(node);
}