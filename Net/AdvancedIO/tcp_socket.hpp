/**
 * 封装一个tcpsocket类，向外提供简单接口能够实现客户端服务端编程流程
 * 1、创建套接字
 * 2、绑定地址信息
 * 3、开始监听/发起连接请求
 * 4、获取已完成连接
 * 5、发送数据
 * 6、接收数据
 * 7、关闭套接字
 **/

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define CHECK_RET(q) if(q == false) {return -1;}
class TcpSocket
{
  public:
    TcpSocket()
    {

    }
    ~TcpSocket()
    {
      //Close();
    }
    //创建套接字
    bool Socket()
    {
      //这里首先创建的时皮条套接字
      _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(_sockfd < 0)
      {
        std::cerr << "socket error" << std::endl;
        return false;
      }
      return true;
    }
    //绑定地址信息
    bool Bind(const std::string& ip, uint16_t port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(&ip[0]);
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
      if(ret < 0)
      {
        std::cerr << "bind error" << std::endl;
        return false;
      }
      return true;
    }
    //服务端开始监听
    bool Listen(int backlog = 5)
    {
      int ret = listen(_sockfd, backlog);
      if(ret < 0)
      {
        std::cerr << "listen error" << std::endl;
        return false;
      }
      return true;
    }
    //连接服务端
    bool Connect(const std::string& ip, uint16_t port)
    {
      int ret;
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(&ip[0]);
      socklen_t len = sizeof(struct sockaddr_in);
      ret = connect(_sockfd, (struct sockaddr*)&addr, len);
      if(ret < 0)
      {
        std::cerr << "connet error" << std::endl;
        return false;
      }
      return true;
    }
    //设置套接字
    void SetFd(int fd)
    {
      _sockfd = fd;
    }
    //获取新的套接字
    bool Accept(TcpSocket& newsock)
    {
      struct sockaddr_in addr;
      socklen_t len = sizeof(struct sockaddr_in);
      //这里fd是皮条套接字新创建出来的连接套接字
      int fd = accept(_sockfd, (struct sockaddr*)&addr, &len);
      if(fd < 0)
      {
        std::cerr << "accept error" << std::endl;
        return false;
      }
      //newsock._sockfd = fd;
      newsock.SetFd(fd);
      return true;
    }
    //发送数据
    bool Send(const std::string& buf)
    {
      int ret = send(_sockfd, &buf[0], buf.size(), 0);
      if(ret < 0)
      {
        std::cerr << "send error" << std::endl;
        return false;
      }
      return true;
    }
    //接收数据
    bool Recv(std::string& buf)
    {
      char tmp[4096] = {0};
      int ret = recv(_sockfd, &tmp[0], 4096, 0);
      if(ret < 0)
      {
        std::cerr << "recv error" << std::endl;
        return false;
      }
      else if(ret == 0)
      {
        std::cerr << "peer shutdown" << std::endl;
        return false;
      }
      buf = tmp;
      return true;
    }
    //关闭
    bool Close()
    {
      if(_sockfd >= 0)
      {
        close(_sockfd);
      }
    }
    int GetFd()
    {
      return _sockfd;
    }
  private:
    int _sockfd;
};
