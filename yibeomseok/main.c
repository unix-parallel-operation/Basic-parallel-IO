#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Basic parallel I/O
 * 각 compute node가 주어진 data들을 개별로 I/O node에 전송하는 방법
 *
 * ex)
 * compute node #0가 D(0)와 D(16)은 I/O node #0로 보내고 D(8)과 D(24)는 I/O node #1로 전송한다.
 * 각 I/O node는 받은 data들을 하나씩 저장한다.
 */

int main()
{
}