/**
 * 服务端实现
 * 1、创建套接字
 * 2、绑定地址信息
 * 3、开始监听
 * 4、获取新连接
 * 5、接收数据
 * 6、发送数据
 * 7、关闭套接字
 **/
#include "tcp_socket.hpp"
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
void sigcb(int signo)
{
  //等待任意一个进程退出
  //SIGCHLD信号是一个非可靠信号
  //多个进程同时退出有可能会造成事件丢失，导致有可能有僵尸进程没有被处理
  //因此在一次事件回调中，将能够处理的僵尸进程全都处理掉
  while(waitpid(-1, NULL, WNOHANG) > 0);
}
int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    std::cerr << "./tcp_srv 192.169.11.128 9000" << std::endl;
    return -1;
  }
  signal(SIGCHLD, sigcb);
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  TcpSocket sock;
  CHECK_RET(sock.Socket());
  CHECK_RET(sock.Bind(ip, port));
  CHECK_RET(sock.Listen());
  //这个新的套接字要放在循环外部，否则一次循环结束变量销毁会关闭套接字连接就会断开
  TcpSocket newsock;
  //这里要并行执行，这里使用多进程
  //让主进程继续获得新连接获取
  //子进程负责与客户端通信
  //并且这种处理方式更加稳定，子进程出现问题主进程并不会出现问题
  while(1)
  {
    bool ret = sock.Accept(newsock);
    if(ret == false)
    {
      continue;
    }
    //子进程处理与客户端的数据通信
    if(fork() == 0)
    {
      //这里让子继承再创建一个子进程，本身直接退出，结束父进程的等待
      /*
      if(fork() > 0)
      {
        exit(0);
      }
      */
      //这里处理数据通信的实际上是子进程的子进程，但是子进程已经推出了
      //这个孙子进程会变成孤儿进程，归init进程管理，并且退出不会变成僵尸进程
      while(1)
      {
        std::string buf;
        ret = newsock.Recv(buf);
        if(ret == false)
        {
          std::cerr << "recv error" << std::endl;
          newsock.Close();
          exit(0);
        }
        std::cout << "client say: " << buf << std::endl;
        buf.clear();
        std::cin >> buf;
        newsock.Send(buf);
      }
      newsock.Close();
      exit(0);
    }
    //父进程直接关闭新连接的套接字
    newsock.Close();
    //父进程要等待子进程，防止变成僵尸进程，但是子进程如果一直不结束，父进程会造成阻塞
    //因此这里有两种方式处理
    //1、让子进程再创建孙子进程处理数据通信，子进程直接退出，孙子进程会变成孤儿进程不会变成僵尸进程
    //wait(NULL);
    //2、在子进程退出后会向父进程发送信号，信号会一直等着我们进行处理，因此我们可以通过改写这个信号来回收所有子进程
  }
  sock.Close();
}
