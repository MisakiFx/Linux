#include <iostream>
#include <vector>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
#include "tcp_socket.hpp"
#define MAX_EPOLL 1024
class Epoll
{
  public:
    Epoll()
      :_epfd(-1)
    {

    }
    ~Epoll()
    {

    }
    bool Init()
    {
      _epfd = epoll_create(MAX_EPOLL);
      if(_epfd < 0)
      {
        std::cerr << "create epoll error" << std::endl;
        return false;
      }
      return true;
    }
    bool Add(TcpSocket& sock)
    {
      struct epoll_event ev;
      int fd = sock.GetFd();
      ev.events = EPOLLIN;
      ev.data.fd = fd;
      int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
      if(ret < 0)
      {
        std::cerr << "append monitor error" << std::endl;
        return false;
      }
      return true;
    }
    bool Del(TcpSocket& sock)
    {
      int fd = sock.GetFd();
      int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
      if(ret < 0)
      {
        std::cerr << "remove monitor error" << std::endl;
        return false;
      }
      return true;
    }
    bool Wait(std::vector<TcpSocket>& list, int timeout = 3000)
    {
      struct epoll_event evs[MAX_EPOLL];
      int nfds = epoll_wait(_epfd, evs, MAX_EPOLL, timeout);
      if(nfds < 0)
      {
        std::cerr << "epoll monitor error" << std::endl;
        return false;
      }
      else if(nfds == 0)
      {
        std::cerr << "epoll monitor timeout" << std::endl;
        return false;
      }
      for(int i = 0; i < nfds; i++)
      {
        int fd = evs[i].data.fd;
        TcpSocket sock;
        sock.SetFd(fd);
        list.push_back(sock);
      }
      return true;
    }
  private:
    int _epfd;
};
int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    std::cout << "./tcp_epoll ip port" << std::endl;
    return -1;
  }
  std::string srv_ip = argv[1];
  uint16_t srv_port = atoi(argv[2]);
  TcpSocket lst_sock;
  CHECK_RET(lst_sock.Socket());
  CHECK_RET(lst_sock.Bind(srv_ip, srv_port));
  CHECK_RET(lst_sock.Listen());
  Epoll e;
  CHECK_RET(e.Init());
  CHECK_RET(e.Add(lst_sock));
  while(1)
  {
    std::vector<TcpSocket> list;
    if(e.Wait(list) == false)
    {
      sleep(1);
      continue;
    }
    for(size_t i = 0; i < list.size(); i++)
    {
      //监听套接字
      if(list[i].GetFd() == lst_sock.GetFd())
      {
        TcpSocket cli_sock;
        if(lst_sock.Accept(cli_sock) == false)
        {
          continue;
        }
        CHECK_RET(e.Add(cli_sock));
      }
      else 
      {
        std::string buf;
        if(list[i].Recv(buf) == false)
        {
          list[i].Close();
          continue;
        }
        std::cout << "Client Say:" << buf << std::endl;
        buf.clear();
        std::cin>> buf;
        if(list[i].Send(buf) == false)
        {
          list[i].Close();
          continue;
        }
      }
    }
  }
  lst_sock.Close();
}
