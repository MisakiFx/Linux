all:tcpclient tcpserver
tcpserver:thread_tcp_srv.cpp
	g++ -g $^ -o $@ -pthread
tcpclient:tcp_cli.cpp
	g++ -g $^ -o $@
udpclient:udp_cli.cpp
	g++ -g $^ -o $@
udpserver:udp_srv.c
	gcc -g $^ -o $@
