/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：封装实现一个udpsocket类；向外提供更加容易使用的udp接 
*             口来实现udp通信流程
================================================================*/

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;
class UdpSocket
{
    public:
        UdpSocket():_sockfd(-1){
        }
        ~UdpSocket(){
            Close();
        }
        bool Socket() {
            _sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (_sockfd < 0) {
                std::cerr << "socket error\n";
                return false;
            }
            return true;
        }
        bool Bind(const string &ip, uint16_t port) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                std::cerr << "bind error\n";
                return false;
            }
            return true;
        }
        bool Recv(string &buf, string &ip, uint16_t &port) {
            char tmp[4096];
            struct sockaddr_in peeraddr;
            socklen_t len = sizeof(peeraddr);
            int ret = recvfrom(_sockfd, tmp, 4096, 0, 
                    (struct sockaddr*)&peeraddr, &len);
            if (ret < 0) {
                std::cerr << "recvfrom error\n";
                return false;
            }

            buf.assign(tmp, ret);
            port = ntohs(peeraddr.sin_port);
            //inet_ntoa 将网络字节序的整数IP地址转换为字符串IP地址
            //返回缓冲区首地址，内部实现使用了静态成员变量
            //非线程安全
            ip = inet_ntoa(peeraddr.sin_addr);
            return true;
        }
        bool Send(string &data, string &ip, uint16_t port) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = sendto(_sockfd, &data[0], data.size(), 0,
                    (struct sockaddr*)&addr, len);
            if (ret < 0) {
                std::cerr << "sento error\n";
                return -1;
            }
            return true;
        }
        bool Close() {
            if (_sockfd >= 0) {
                close(_sockfd);
                _sockfd = -1;
            }
            return true;
        }
    private:
        int _sockfd;
};

#define CHECK_RET(q) if((q) == false){return -1;}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "./udp_cli serverip serverport\n";
        return -1;
    }
    std::string srv_ip = argv[1];
    uint16_t srv_port = atoi(argv[2]);
    UdpSocket sock;

    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind("192.168.122.132", 8000));
    while(1) {
        string buf;
        cin >> buf;
        CHECK_RET(sock.Send(buf, srv_ip, srv_port));
        buf.clear();
        CHECK_RET(sock.Recv(buf, srv_ip, srv_port));
        std::cout << "server say:"<<buf<<"\n";
    }
    CHECK_RET(sock.Close());
    return 0;
}
