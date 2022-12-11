#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>

struct cpt_node
{
  int data_fd;
  int size;
  int *datas;
  int msgq_id[2];
};

/* k_size는 전체 바이트를 sizeof(int)(== 4)로 나눈 값에 해당됨. 이것은 정수 개수로 변환하기 위함 */
struct cpt_node get_cpt_node(char *data_path, int k_size, int *msgq_id)
{
  struct cpt_node node;
  int i, n, last = 0, buf[10];

  node.data_fd = open(data_path, O_RDONLY, 0644);
  if (node.data_fd == -1)
  {
    perror("open: cpt_node");
    exit(1);
  }
  node.size = k_size * 1024 / 4; // 총 노드 개수가 4개이기 때문에 4분할 한 것임
  node.datas = (int *)malloc(node.size * sizeof(int));

  /* ------------------not essential code----------------------------- */
  /**
   * 없어도 되는 코드. 제대로 읽었는지 확인하고자 할 때 사용.
   */
  while ((n = read(node.data_fd, buf, 40)) > 0)
  {
    for (i = 0; i < n / 4; i++)
    {
      node.datas[last++] = buf[i];
    }
  }
  lseek(node.data_fd, 0, SEEK_SET);
  /* ----------------------------------------------------------------- */

  node.msgq_id[0] = msgq_id[0];
  node.msgq_id[1] = msgq_id[1];

  return node;
}

void close_cpt_node(struct cpt_node node)
{
  free(node.datas);
  close(node.data_fd);
}