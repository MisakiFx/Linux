//用select模型实现并发服务器
//对select进行封装
#include <iostream>
#include <vector>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <sys/select.h>
#include "tcp_socket.hpp"
class Select
{
  public:
    Select():_maxfd(-1)
    {
      FD_ZERO(&_rfds);
    }
    //添加要监听的套接字
    bool Add(TcpSocket& sock)
    {
      int fd = sock.GetFd();
      FD_SET(fd, &_rfds);
      _maxfd = _maxfd > fd ? _maxfd : fd;
      return true;
    }
    //监听，list返回就绪的描述符，sec为默认等待时间
    bool Wait(std::vector<TcpSocket>& list, int sec = 3)
    {
      struct timeval tv;
      tv.tv_sec = sec;
      tv.tv_usec = 0;
      fd_set tmp_set = _rfds;//每次定义新的集合进行监控，为了避免原有的监控集合被修改
      int count = select(_maxfd + 1, &tmp_set, NULL, NULL, &tv);
      if(count < 0)
      {
        std::cout << "select error" << std::endl;
        return false;
      }
      else if(count == 0)
      {
        std::cout << "wait timeout" << std::endl;
        return false;
      }
      for(int i = 0; i <= _maxfd; i++)
      {
        if(FD_ISSET(i, &tmp_set))
        {
          TcpSocket sock;
          sock.SetFd(i);
          list.push_back(sock);
        }
      }
      return true;
    }
    //删除不用再监听的套接字
    bool Del(TcpSocket& sock)
    {
      int fd = sock.GetFd();
      FD_CLR(fd, &_rfds);
      for(int i = _maxfd; i >= 0; i--)
      {
        if(FD_ISSET(i, &_rfds))
        {
          _maxfd = i;
          return true;
        }
      }
      _maxfd = -1;
      return true;
    }
  private:
    fd_set _rfds;//读事件描述符集合
    int _maxfd;//最大描述符
};
int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    std::cout << "./tcpselect ip port" << std::endl;
    return -1;
  }
  TcpSocket sock;
  std::string srv_ip = argv[1];
  uint16_t srv_port = atoi(argv[2]);
  CHECK_RET(sock.Socket());
  CHECK_RET(sock.Bind(srv_ip, srv_port));
  CHECK_RET(sock.Listen());
  Select s;
  s.Add(sock);
  while(1)
  {
    std::vector<TcpSocket> list;
    if(!s.Wait(list))
    {
      sleep(1);
      continue;
    }
    for(auto clisock : list)
    {
      if(clisock.GetFd() == sock.GetFd())//监听套接字
      {
        TcpSocket socktmp;
        if(sock.Accept(socktmp) == false)
        {
          continue;
        }
        s.Add(socktmp);
      }
      else //通信套接字
      {
        std::string buf;
        if(clisock.Recv(buf) == false)
        {
          s.Del(clisock);
          clisock.Close();
          continue;
        }
        std::cout << "client say:" << buf << std::endl;
        buf.clear();
        std::cin >> buf;
        if(clisock.Send(buf) == false)
        {
          s.Del(clisock);
          clisock.Close();
          continue;
        }
      }
    }
  }
  sock.Close();
}
