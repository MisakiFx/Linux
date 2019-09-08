/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：线程池的实现：线程安全的任务队列+线程
*       任务类:
*           class MyTask{}
*       线程池类
*           class ThreadPool {} 
================================================================*/
#include <iostream>
#include <sstream>
#include <thread>
#include <queue>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX_THREAD  5
#define MAX_QUEUE   10

typedef void (*handler_t)(int);

//任务类中有两个成员：
//  1. 要处理的数据
//  2. 数据的处理方法
//这两个成员都需要用户传入
//  一个线程中用什么方法处理什么数据都有用户自己决定
class MyTask {
    private:
        int _data;
        handler_t _handler;
    public:
        MyTask(int data, handler_t handle) : 
            _data(data), _handler(handle) {}

        void SetTask(int data, handler_t handle) { 
            _data = data; 
            _handler = handle; 
            return;
        }
        void Run() {   
            return _handler(_data);  
        }
};
class ThreadPool {
    private:
        std::queue<MyTask > _queue; 
        int _capacity;  
        pthread_mutex_t _mutex;   
        pthread_cond_t _cond_pro; 
        pthread_cond_t _cond_con;

        int _thr_max;   //用于控制线程的最大数量
        bool _quit_flag; //用于控制线程退出
        int _thr_cur;   //线程退出时，判断当前线程数量
        std::vector<std::thread> _thr_list;   //存储线程操作句柄
    private:
        void thr_start(){
            while(1) {
                pthread_mutex_lock(&_mutex);
                while(_queue.empty()) {
                    if (_quit_flag == true) {
                        std::cout << "thread exit " <<pthread_self() << std::endl;
                        pthread_mutex_unlock(&_mutex);
                        _thr_cur--;
                        return;
                    }
                    pthread_cond_wait(&_cond_con, &_mutex);
                }
                MyTask tt = _queue.front();
                _queue.pop();
                pthread_mutex_unlock(&_mutex);
                pthread_cond_signal(&_cond_pro);
                //任务处理应该放在解锁之后,否则会造成,同一时间只有
                //一个线程在处理任务
                tt.Run();
            }
            return;
        }
    public: 
        ThreadPool (int maxq = MAX_QUEUE, int maxt = MAX_THREAD):
            _capacity(maxq), _thr_max(maxt), _thr_list(maxt),
            _quit_flag(false) {
            pthread_mutex_init(&_mutex, NULL);
            pthread_cond_init(&_cond_con, NULL);
            pthread_cond_init(&_cond_pro, NULL);
        }
        ~ThreadPool() {
            pthread_mutex_destroy(&_mutex);
            pthread_cond_destroy(&_cond_con);
            pthread_cond_destroy(&_cond_pro);
        }
        bool PoolInit() {
            _thr_cur = 0;
            for (int i = 0; i < _thr_max; i++) {
                _thr_list[i] = std::thread(&ThreadPool::thr_start, this);
                _thr_cur++;
                _thr_list[i].detach();
            }
            return true;
        }
        bool AddTask(MyTask &tt) {
            pthread_mutex_lock(&_mutex);
            while(_queue.size() == _capacity) {
                pthread_cond_wait(&_cond_pro, &_mutex);
            }
            _queue.push(tt);
            pthread_mutex_unlock(&_mutex);
            pthread_cond_signal(&_cond_con);
            return true;
        }
        bool PoolStop() {
            pthread_mutex_lock(&_mutex);
            _quit_flag = true;
            pthread_mutex_unlock(&_mutex);
            
            while(_thr_cur > 0) {
                pthread_cond_broadcast(&_cond_con);
                usleep(1000);
            }
            return true;
        }
};

void test(int data) {
    srand(time(NULL));
    int nsec = rand() % 5;
    std::stringstream ss;
    ss << "thread:" << pthread_self() << "processing data ";
    ss << data << "and sleep " << nsec << " sec\n"; 

    std::cout << ss.str();
    sleep(nsec);
    return;
}
int main() 
{
    ThreadPool pool;
    pool.PoolInit();
    for (int i = 0; i < 10; i++) {
        MyTask tt(i, test);
        pool.AddTask(tt);
    }

    pool.PoolStop();
    return 0;
}
