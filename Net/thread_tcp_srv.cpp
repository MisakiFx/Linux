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
#include <pthread.h>
//线程入口函数，创建线程进行数据通信
void* thr_start(void* arg)
{
  TcpSocket* newsock = (TcpSocket*)arg;
  while(1)
  {
    std::string buf;
    bool ret = newsock->Recv(buf);
    if(ret == false)
    {
      std::cerr << "recv error" << std::endl;
      newsock->Close();
      return NULL;
    }
    std::cout << "client say: " << buf << std::endl;
    buf.clear();
    std::cin >> buf;
    newsock->Send(buf);
  }
  newsock->Close();
  delete newsock;
  return NULL;
}
int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    std::cerr << "./tcp_srv 192.169.11.128 9000" << std::endl;
    return -1;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  TcpSocket sock;
  CHECK_RET(sock.Socket());
  CHECK_RET(sock.Bind(ip, port));
  CHECK_RET(sock.Listen());
  while(1)
  {
    //为了避免内存泄露，描述符覆盖等问题
    TcpSocket* newsock = new TcpSocket();
    bool ret = sock.Accept(*newsock);
    if(ret == false)
    {
      //连接失败别忘了也要把空间销毁，以免内存泄露
      delete newsock;
      continue;
    }
    pthread_t tid;
    pthread_create(&tid, NULL, thr_start, (void*)newsock);
    pthread_detach(tid);
  }
  sock.Close();
}
