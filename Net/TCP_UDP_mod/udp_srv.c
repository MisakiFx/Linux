/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：传输层基于UDP协议的服务端程序
*       1.  创建套接字
*       2.  为套接字绑定地址信息
*       3.  接收数据
*       4.  发送数据
*       5.  关闭套接字
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: ./main 192.168.122.132 9000\n");
        return -1;
    }
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket error");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    //in_addr_t inet_addr(const char *cp);
    //将字符串点分十进制IP地址转换为网络字节序IP地址
    addr.sin_addr.s_addr = inet_addr("192.168.122.132");

    socklen_t len = sizeof(struct sockaddr_in);
    int ret = bind(sockfd, (struct sockaddr*)&addr, len);
    if (ret < 0) {
        perror("bind error");
        return -1;
    }

    while(1) {
        //接收数据
        char buf[1024] = {0};
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(struct sockaddr_in);
        int ret = recvfrom(sockfd, buf, 1023, 0, 
                (struct sockaddr*)&cliaddr, &len);
        if (ret < 0) {
            perror("recvfrom error");
            close(sockfd);
            return -1;
        }
        printf("client say: %s\n", buf);
        //sendto
        
        len = sizeof(struct sockaddr_in);
        memset(buf, 0x00, 1024);
        scanf("%s", buf);
        ret = sendto(sockfd, buf, strlen(buf), 0, 
                (struct sockaddr*)&cliaddr, len);
        if (ret < 0) {
            perror("sendto error");
            close(sockfd);
            return -1;
        }
    }
    close(sockfd);
}
