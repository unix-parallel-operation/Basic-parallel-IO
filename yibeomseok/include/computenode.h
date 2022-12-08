#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>

struct cpt_node {
  int data_fd;
  int size;
  int* datas;
  int msgq_id[2];
};

struct cpt_node get_cpt_node(char* data_path, int k_size, int* msgq_id) {
  struct cpt_node node;
  
  node.data_fd = open(data_path, O_RDONLY, 0644);
  if(node.data_fd == -1) {
    perror("open: cpt_node");
    exit(1);
  }
  node.size = k_size * 1024 / 4;  // 총 노드 개수가 4개이기 때문에 4분할 한 것임
  node.datas = (int*)malloc(node.size * sizeof(int));
  
  int i, n, last = 0, buf[10];
  while((n = read(node.data_fd, buf, 40)) > 0 ) {
    for(i = 0; i < n/4; i++) {
      node.datas[last++] = buf[i];
    }
  }

  lseek(node.data_fd, 0, SEEK_SET);
  node.msgq_id[0] = msgq_id[0];
  node.msgq_id[1] = msgq_id[1];
  
  return node;
}

void close_cpt_node(struct cpt_node node) {
  free(node.datas);
  close(node.data_fd);
}


/**
 * 재구성
 * 
 * 노드 구조체
 * 1. 읽을 파일을 가리키는 fd
 * 2. 보낼 IO node를 가리키는 무언가 (메시지 큐에 줘도 되고 소켓으로 줘도 되고)
 * 2. 소켓으로 줄까...
 * 3. 
 * 출구: 0번
 * 0 ~ 1023
 * 
 * 출구: 1번
 * 1024 ~ 2047
 * 
 * 출구: 0번
 * 2048 ~ 3071
 * 
*/