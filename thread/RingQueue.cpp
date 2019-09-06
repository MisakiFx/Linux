/**
 * 利用信号量完成线程安全的环形队列
 **/ 
#include <iostream>
#include <vector>
#include <semaphore.h>
#include <thread>
#define MAXQ 10
class RingQueue
{

public:
  RingQueue(int maxq = MAXQ)
    :_capacity(maxq)
    ,_queue(maxq)
    ,_step_read(0)
    ,_step_write(0)
  {
    sem_init(&_lock, 0, 1);
    sem_init(&_idle_space, 0, maxq);
    sem_init(&_data_space, 0, 0);
  }
  ~RingQueue()
  {
    sem_destroy(&_lock);
    sem_destroy(&_idle_space);
    sem_destroy(&_data_space);
  }
  bool QueuePush(int data)
  {
    //没有空闲空间则阻塞
    sem_wait(&_idle_space);
    //加锁
    sem_wait(&_lock);
    _queue[_step_write] = data;
    _step_write = (_step_write + 1) % _capacity;
    //解锁
    sem_post(&_lock);
    //唤醒消费者
    sem_post(&_data_space);
    return true;
  }
  bool QueuePop(int& data)
  {
    sem_wait(&_data_space);
    sem_wait(&_lock);
    data = _queue[_step_read];
    _step_read = (_step_read + 1) % _capacity;
    sem_post(&_lock);
    sem_post(&_idle_space);
    return true;
  }
private:
  std::vector<int> _queue;//用vector实现环形队列
  int _capacity;//容量
  int _step_read;//读指针
  int _step_write;//写指针
  
  sem_t _lock;//初始计数=1，负责完成互斥

  //也需要有两个等待队列，分别完成两个角色间的同步
  sem_t _idle_space;//空闲空间节点个数，生产者等待在这里，完成同步
  sem_t _data_space;//数据节点个数，初始=0，消费者等待在这里，完成同步
};
void thr_producer(RingQueue* q)
{
  int data = 0;
  while(1)
  {
    q->QueuePush(data);
    std::cout << "push data ----" << data++ << std::endl;
  }
}
void thr_consumer(RingQueue* q)
{
  int data = 0;
  while(1)
  {
    q->QueuePop(data);
    std::cout << "get data ----" << data << std::endl;
  }
}

int main()
{
  RingQueue q;
  std::vector<std::thread> list_con(4);
  std::vector<std::thread> list_pro(4);
  for(int i = 0; i < 4; i++)
  {
    list_pro[i] = (std::thread(thr_producer, &q));
  }
  for(int i = 0; i < 4; i++)
  {
    list_con[i] = (std::thread(thr_consumer, &q));
  }
  for(int i = 0; i < 4; i++)
  {
    list_con[i].join();
    list_pro[i].join();
  }
}
