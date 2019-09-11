/**
 * 传输层基于UDP协议的服务端程序
 * 1、创建套接字
 * 2、为套接字绑定地址信息
 * 3、接收数据
 * 4、发送数据
 * 5、关闭套接字
 **/
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    printf("Usage: ./main 192.168.122.132 9000\n");
    return -1;
  }
  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(sockfd < 0)
  {
    perror("socket error\n");
    return -1;
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[2]));
  //inet_addr将点分十进制ip地址转换为网络字节序ip地址
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  socklen_t len = sizeof(struct sockaddr_in);
  int ret = bind(sockfd, (struct sockaddr*)&addr, len);
  if(ret < 0)
  {
    perror("bind error");
    return -1;
  }
  while(1)
  {

  }
  close(sockfd);
}
