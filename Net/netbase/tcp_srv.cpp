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
  //这个新的套接字要放在循环外部，否则一次循环结束变量销毁会关闭套接字连接就会断开
  TcpSocket newsock;
  while(1)
  {
    bool ret = sock.Accept(newsock);
    if(ret == false)
    {
      continue;
    }
    calc_t buf;
    ret = newsock.Recv((void*)&buf, sizeof(calc_t));
    if(ret == false)
    {
      std::cerr << "Recv error" << std::endl;
      return -1;
    }
    std::cout << buf.num1 << " " << buf.num2 << " " << buf.op << std::endl;
  }
  sock.Close();
}
