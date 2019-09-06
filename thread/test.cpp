#include <iostream>
#include <thread>
#include <vector>
#include <semaphore.h>

#define MAXQ 10

class RingQueue
{
  public:
    RingQueue(int maxq = MAXQ):_capacity(maxq), _queue(maxq), _step_read(0), _step_write(0){
      sem_init(&_lock, 0, 1);
      sem_init(&_idle_space, 0, maxq);
      sem_init(&_data_space, 0, 0);

    }
    ~RingQueue(){
      sem_destroy(&_lock);
      sem_destroy(&_data_space);
      sem_destroy(&_idle_space);

    }
    bool QueuePush(int data) {
      sem_wait(&_idle_space);
      sem_wait(&_lock);
      _queue[_step_write] = data;
      _step_write = (_step_write + 1) % _capacity;
      sem_post(&_lock);
      sem_post(&_data_space);
      return true;

    }
    bool QueuePop(int &data) {
      sem_wait(&_data_space);
      sem_wait(&_lock);
      data = _queue[_step_read];
      _step_read = (_step_read + 1) % _capacity;
      sem_post(&_lock);
      sem_post(&_idle_space);
      return true;

    }
  private:
    std::vector<int> _queue;
    int _capacity;
    int _step_read;
    int _step_write;
    sem_t _lock;
    sem_t _idle_space;
    sem_t _data_space;

};

void thr_producer(RingQueue *q) 
{
  int data = 0;
  while(1){
    q->QueuePush(data);
    std::cout << "put data --------" << data++ << std::endl;

  }
  return;

}
void thr_consumer(RingQueue *q) 
{
  int data = 0;
  while(1){
    q->QueuePop(data);
    std::cout << "get data ----"<< data << std::endl;

  }
  return;

}
int main()
{
  RingQueue q;
  std::vector<std::thread> list_con(4);
  std::vector<std::thread> list_pro(4);
  for (int i = 0; i < 4; i++) {
    list_pro[i] = std::thread (thr_producer, &q);

  }
  for (int i = 0; i < 4; i++) {
    list_con[i] = std::thread (thr_consumer, &q);

  }

  for (int i = 0; i < 4; i++) {
    list_pro[i].join();
    list_con[i].join();

  }

  return 0;

}
