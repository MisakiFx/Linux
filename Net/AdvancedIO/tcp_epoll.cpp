#include <iostream>
#include <vector>
#include "tcp_socket.hpp"
class Epoll
{
  public:
    bool Add(TcpSocket& sock);
    bool Del(TcpSocket& sock);
    bool Wait(std::vector<TcpSocket> list);
  private:
    int _epollfd;
};
