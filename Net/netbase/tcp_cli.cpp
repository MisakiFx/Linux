#include "tcp_socket.hpp"
#include <stdlib.h>
/**
 * 实现客户端
 * 1、创建套接字
 * 2、绑定地址信息（客户端不需要手动绑定）
 * 3、向服务端发起连接请求
 * 4、发送数据
 * 5、接收数据
 * 6、关闭套接字
 **/
int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    std::cerr << "./tcp_cli srvip srvport" << std::endl;
    return -1;
  }
  TcpSocket sock;
  CHECK_RET(sock.Socket());
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  CHECK_RET(sock.Connect(ip, port));
  while(1)
  {
    calc_t tmp;
    tmp.num1 = 11;
    tmp.num2 = 22;
    tmp.op = '+';
    sock.Send((void*)&tmp, sizeof(calc_t));
  }
  sock.Close();
}
