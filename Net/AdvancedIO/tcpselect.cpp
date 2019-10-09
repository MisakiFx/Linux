//用select模型实现并发服务器
//对select进行封装
#include <iostream>
#include <vector>
#include <unistd.h>
class Select
{
  public:
    bool Add(TcpSocket& sock)
    {
      int fd = sock.GetFd();
      FD_SET(fd, &_rfds);
      _maxfd = _maxfd > fd ? _maxfd : fd;
      return true;
    }
    bool wait(std::vector<TcpSocket>& list, int sec = 3)
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
    bool Del(TcpSocket& sock)
    {
      int fd = sock.GetFd();
      FD_CLR(fd, &_rfds);
      for(int i = _maxfd; i >= 0; i--)
      {
        if(FD_ISSET(i, &_rfds))
        {
          _maxfd = i;
          break;
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
