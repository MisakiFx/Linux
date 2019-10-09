all:tcpselect tcpclient
tcpselect:tcp_select.cpp
	g++ -std=c++11 $^ -o $@
tcpclient:tcp_cli.cpp 
	g++ -std=c++11 $^ -o $@
select:select.c
	gcc -g $^ -o $@
