threadpool:ThreadPool.cpp
	g++ -std=c++11 -g $^ -o $@ -pthread
ringqueue:RingQueue.cpp
	g++ -std=c++11 -g $^ -o $@ -pthread
consumer:BlockQueue.cpp
	g++ -std=c++11 -g $^ -o $@ -pthread
cond:thread_cond.c
	gcc -g thread_cond.c -o cond -pthread
main:thread_lock.c
	gcc -g thread_lock.c -o main -pthread
