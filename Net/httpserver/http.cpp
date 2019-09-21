/**
 * 实现一个简单的http服务器
 **/
#include "tcp_socket.hpp"
#include <iostream>
#include <sstream>
int main()
{
  TcpSocket sock;
  CHECK_RET(sock.Socket());
  CHECK_RET(sock.Bind("0.0.0.0", 9000));
  CHECK_RET(sock.Listen());
  while(1)
  {
    TcpSocket cliSock;
    if(sock.Accept(cliSock) == false)
    {
      continue;
    }
    std::string buf;
    cliSock.Recv(buf);
    std::cout << "req:[" << buf << "]" << std::endl;;
    std::string body = "<html><body><h1>学习是一种态度</h1></body></html>";
    body += "<meta http-equiv='content-type' content='text/html;charset=utf-8'>";
    //std::string first = "HTTP/1.1 200 OK";
    //这类改为重定向
    //std::string first = "HTTP/1.1 302 OK";
    //这次改为客户端错误
    std::string first = "HTTP/1.1 404 OK";
    std::stringstream ss;
    ss << "Content-Length: " << body.size() << "\r\n";
    ss << "Content-Type: " << "text/html" << "\r\n";
    ss << "Location: http://www.taobao.com/\r\n";
    std::string head = ss.str();
    std::string blank = "\r\n";
    cliSock.Send(first);
    cliSock.Send(head);
    cliSock.Send(blank);
    cliSock.Send(body);
    cliSock.Close();
  }
  sock.Close();
}
