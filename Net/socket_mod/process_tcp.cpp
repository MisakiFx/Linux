#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "tcpsocket.hpp"

void sigcb(int signo)
{
    //SIGCHLD信号是一个非可靠信号，多个进程同时退出，
    //有可能会造成事件丢失，导致有可能会有僵尸进程没有被处理
    //因此在一次事件回调中，将能够处理的僵尸进程全都处理掉
    while(waitpid(-1, NULL, WNOHANG) > 0);
}
int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "./tcp_srv 192.168.122.132 9000\n";
        return -1;
    }
    signal(SIGCHLD, sigcb);
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    TcpSocket lst_sock;
    //1. 创建套接字
    CHECK_RET(lst_sock.Socket());
    //2. 绑定地址信息
    CHECK_RET(lst_sock.Bind(ip, port));
    //3. 开始监听
    CHECK_RET(lst_sock.Listen());
    TcpSocket newsock;
    while(1) {
        //4. 获取已完成新连接
        bool ret = lst_sock.Accept(newsock);
        if (ret == false) {
            continue;
        }
        //创建子进程进行任务处理
        //1. 任务分摊--每个 进程只负责一个任务（主进程负责新连接获取
        //子进程负责与客户端进行通信）
        //2. 稳定性高--子进程处理请求出问题不会影响到主进程服务器
        if (fork() == 0) {
            /*
            if (fork() > 0) {
                exit(0);
            }*/
            while(1) {
                std::string buf;
                ret = newsock.Recv(buf);
                if (ret == false) {
                    newsock.Close();
                    exit(0);
                }
                std::cout << "client say:" << buf << "\n";
                buf.clear();
                std::cin >> buf;
                newsock.Send(buf);
            }
            newsock.Close();
            exit(0);
        }
        newsock.Close();
    }
    //7. 关闭套接字
    lst_sock.Close();
    return 0;
}
