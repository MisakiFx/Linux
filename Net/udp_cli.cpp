/**
 * 封装实现一个udpsocjet类，向外提供更加容易使用的udp接口
 **/
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define CHECK_RET(q) if((q) == false){return -1;}
using std::string;
class UdpSocket
{
  public:
    UdpSocket()
      :_sockfd(-1)
    {

    }
    ~UdpSocket()
    {
      Close();
    }
    //创建套接字
    bool Socket()
    {
      _sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if(_sockfd < 0)
      {
        std::cerr << "socket error" << std::endl;
        return false;
      }
      return true;
    }
    //绑定本机ip地址及端口信息
    bool Bind(const string& ip, uint16_t port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
      if(ret < 0)
      {
        std::cerr << "bind error" << std::endl;
        return false;
      }
      return true;
    }
    //接收数据并接收对端的ip地址及端口信息
    bool Recv(string& buf, string& ip, uint16_t& port)
    {
      char tmp[4096];
      struct sockaddr_in peeraddr;
      socklen_t len = sizeof(peeraddr);
      int ret = recvfrom(_sockfd, tmp, 4096, 0, (struct sockaddr*)&peeraddr, &len);
      if(ret < 0)
      {
        std::cerr << "recvfrom error" << std::endl;
        return false;
      }
      buf.assign(tmp, ret);
      port = ntohs(peeraddr.sin_port);
      ip = inet_ntoa(peeraddr.sin_addr);
      return true;
    }
    //发送数据
    bool Send(const string& data, const string& ip, const uint16_t& port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = sendto(_sockfd, &data[0], data.size(), 0, (struct sockaddr*)&addr, len);
      if(ret < 0)
      {
        std::cerr << "send error" << std::endl;
        return false;
      }
      return true;
    }
    bool Close()
    {
      if(_sockfd >= 0)
      {
        close(_sockfd);
        _sockfd = -1;
      }
      return true;
    }
  private:
    int _sockfd;
};
int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    std::cerr << "./udp_cli serverip serverport" << std::endl;
    return -1;
  }
  UdpSocket sock;
  CHECK_RET(sock.Socket());
  //CHECK_RET(sock.Bind("192.168.11.128", 8000));
  while(1)
  {
    string buf;
    std::cin >> buf;
    CHECK_RET(sock.Send(buf, argv[1], atoi(argv[2])));
    buf.clear();
    string ip;
    uint16_t port;
    CHECK_RET(sock.Recv(buf, ip, port));
    std::cout << "server say: " << buf << std::endl;
  }
}
