/**
 * 线程池由两个部分构成一个是一个任务类
 * 另一个部分是一个线程安全的队列，由此构成任务队列，
 * 再用一组线程从任务队列中获取任务来执行
 * 由此构成线程池
 **/
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <queue>
#include <thread>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <sstream>
#define MAX_THREAD 5
#define MAX_QUEUE 10
typedef void(*handler_t)(int val);
//任务类
//1、决定线程处理的任务，处理什么数据，怎么处理都由用户传入
class Task
{
  private:
    int _data;//数据
    handler_t _handler;//处理数据的方法，函数指针，用于传入线程中给线程下达命令
  public:
    Task(int data, handler_t handler)
      :_data(data)
      ,_handler(handler)
    {

    }
    void SetTask(int data, handler_t handler)
    {
      _data = data;
      _handler = handler;
    }
    void Run()
    {
      return _handler(_data);
    }

};
//线程池类
class ThreadPool
{
  private:
    std::queue<Task> _queue;//任务队列
    int _capacity;//线程池最大任务数量
    pthread_mutex_t _mutex;//锁，完成互斥，类似于生产者消费者模型
    pthread_cond_t _cond_pro;//条件变量，完成
    pthread_cond_t _cond_con;
    int _thr_max;//线程池拥有的总线程数
    std::vector<std::thread> _thr_list;//线程组，存储线程操作句柄
    bool _quit_flag;//用于控制线程是否退出
    int _thr_cur;//线程的数量，线程退出时，判断当前线程数量
    void thr_start()
    {
      while(1)
      {
        pthread_mutex_lock(&_mutex);
        while(_queue.empty())
        {
          if(_quit_flag == true)
          {
            std::cout << "thread exit " << pthread_self() << std::endl; 
            pthread_mutex_unlock(&_mutex);
            _thr_cur--;
            return;
          }
          pthread_cond_wait(&_cond_con, &_mutex);
        }
        Task tt = _queue.front();
        _queue.pop();
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&_cond_pro);
        //任务处理放到锁外，防止线程处理任务时间过长，一直加锁导致其他线程无法处理其他任务
        tt.Run();
      }
    }
  public:
    //初始化线程池
    ThreadPool(int maxq = MAX_QUEUE, int maxt = MAX_THREAD)
      :_capacity(maxq)
      ,_thr_max(maxt)
      ,_thr_list(maxt)
      ,_thr_cur(0)
    {
      pthread_mutex_init(&_mutex, NULL);
      pthread_cond_init(&_cond_pro, NULL);
      pthread_cond_init(&_cond_con, NULL);
    }
    ~ThreadPool()
    {
      pthread_mutex_destroy(&_mutex);
      pthread_cond_destroy(&_cond_pro);
      pthread_cond_destroy(&_cond_con);
    }
    //初始化线程组
    bool PoolInit()
    {
      _thr_cur = 0;
      for(int i = 0; i < _thr_max; i++)
      {
        _thr_list[i] = std::thread(&ThreadPool::thr_start, this);
        _thr_cur++;
        _thr_list[i].detach();
      }
      return true;
    }
    //添加任务
    bool AddTask(Task& tt)
    {
      pthread_mutex_lock(&_mutex);
      while(_queue.size() == _capacity)
      {
        pthread_cond_wait(&_cond_pro, &_mutex);
      }
      _queue.push(tt);
      pthread_mutex_unlock(&_mutex);
      pthread_cond_signal(&_cond_con);
      return true;
    }
    //销毁线程池，停止工作
    bool PoolStop()
    {
      pthread_mutex_lock(&_mutex);
      _quit_flag = true;
      pthread_mutex_unlock(&_mutex);
      while(_thr_cur > 0)
      {
        pthread_cond_broadcast(&_cond_con);
        usleep(1000);
      }
      //for(int i = 0; i < _thr_max; i++)
      //{
      //  _thr_list[i].join();
      //}
      return true;
    }
};
void test(int data)
{
  srand(time(NULL));
  int nsec = rand() % 5;
  std::stringstream ss;
  ss << "thread:" << pthread_self() << " processint data ";
  ss << data << "and sleep " << nsec << " sec" << std::endl;  
  std:: cout << ss.str();
  sleep(nsec);
  return;
}
int main()
{
  ThreadPool pool;
  pool.PoolInit();
  for(int i = 0; i < 10; i++)
  {
    Task tt(i, test);
    pool.AddTask(tt);
  }
  pool.PoolStop();
}
