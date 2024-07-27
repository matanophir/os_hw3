#ifndef TASKQ
#define TASKQ

#include "queue.h"
#include <pthread.h>

typedef enum  {
    BLOCK = 1,
    DT,
    DH,
    BF,
    RANDOM
} Policy;

typedef struct Taskq
{
    Policy policy;
    Queue *waiting;
    Queue *running;
    int max_size;
    pthread_mutex_t *lock;
    pthread_cond_t *has_room;
    pthread_cond_t *has_elements;
    pthread_cond_t *empty;

    pthread_mutex_t _lock; // easier to use the pointers..
    pthread_cond_t _has_room;
    pthread_cond_t _has_elements;
    pthread_cond_t _empty;
} Taskq;


Taskq* createTaskq(int max_size, char* policy);

void add_task(Taskq *q, Task task);

Task* get_task(Taskq *q); 

Task* get_last_task(Taskq *q);

Task mark_done(Taskq* q, Task *task);

void _fill_dispatch(Task* task);
void _fill_finished(Task* task);


#endif //TASKQ