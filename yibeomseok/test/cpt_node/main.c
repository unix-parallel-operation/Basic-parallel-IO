#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/computenode.h"

/**
 * Basic parallel I/O
 * 각 compute node가 주어진 data들을 개별로 I/O node에 전송하는 방법
 * 
 * ex)
 * compute node #0가 D(0)와 D(16)은 I/O node #0로 보내고 D(8)과 D(24)는 I/O node #1로 전송한다.
 * 각 I/O node는 받은 data들을 하나씩 저장한다.
*/

int main() {
  struct cpt_node node = get_cpt_node("data0.dat", 16);

  int i = 0;
  for(i = 0; i < node.size; i++) {
    printf("%d ", node.datas[i]);
    if(i != 0 && i % 20 == 0) printf("\n");
  }

  close_cpt_node(node);

  return 0;
}