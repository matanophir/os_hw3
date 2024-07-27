#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "taskq.h"
#include "segel.h"

#define container_of(ptr, type, member) \
 ((type *)                              \
   (  ((char *)(ptr))                   \
    - ((char *)(&((type*)0)->member)) ))

Taskq* createTaskq(int max_size, char *policy)
{
    Taskq *taskq = (Taskq*)malloc(sizeof(Taskq));
    taskq->waiting = createQueue();
    taskq->running = createQueue();
    taskq->max_size = max_size;
    pthread_cond_init(&taskq->_has_room, NULL);
    pthread_cond_init(&taskq->_has_elements, NULL);
    pthread_cond_init(&taskq->_empty, NULL);
    pthread_mutex_init(&taskq->_lock, NULL);
    taskq->has_room = &taskq->_has_room;
    taskq->has_elements = &taskq->_has_elements;
    taskq->empty = &taskq->_empty;
    taskq->lock = &taskq->_lock;

    if (strcmp(policy, "block") == 0){
        taskq->policy = BLOCK;

    }else if (strcmp(policy, "dt") == 0) {
        taskq->policy = DT;
    } else if (strcmp(policy, "dh") == 0) {
        taskq->policy = DH;
    } else if (strcmp(policy, "bf") == 0) {
        taskq->policy = BF;
    } else if (strcmp(policy, "random") == 0) {
        taskq->policy = RANDOM;
    }
    // else {
    //     printf ("no such policy"); // will never get here because checking in the init
    //     taskq->policy = BLOCK;
    // }

    return taskq;
}

void add_task(Taskq *q, Task task)
{
    pthread_mutex_lock(q->lock);

    if ( q->waiting->size + q->running->size == q->max_size) // overload check
    {
        switch (q->policy)
        {
        case 1 : //block
            while (q->waiting->size + q->running->size == q->max_size)
            {
                pthread_cond_wait(q->has_room, q->lock);
            }
            break;
        case 2 : //dt
            Close(task.fd);
            goto ret;
            break;
        case 3: // dh
            if (q->waiting->size != 0)
            {
                Task old_task = dequeue(q->waiting);
                Close(old_task.fd);
            }else
            {
                // while (q->waiting->size + q->running->size == q->max_size)
                // {
                //     pthread_cond_wait(q->has_room, q->lock);
                // }
                Close(task.fd); //TODO how should hanlde?
                goto ret;
            }
            break;
        case 4 : //bf
            while (q->waiting->size + q->running->size == q->max_size)
            {
                pthread_cond_wait(q->empty, q->lock);
            }
            Close(task.fd);
            goto ret;
            break;
        case 5 : //random
        if (q->waiting->size == 0){
            // while (q->waiting->size + q->running->size == q->max_size)
            // {
            //     pthread_cond_wait(q->empty, q->lock);
            // }
            // break;
            Close(task.fd); // TODO what should i do in this case? drop or wait?
            goto ret;
        }
        int *marks = calloc(q->waiting->size, sizeof(int));
        int n_marks = 0;
        int needed_marks = (q->waiting->size + 1) / 2;
        int pos = 0;
        while (n_marks != needed_marks)
        {
            if (marks[pos] == 0){
                if (rand()%2 == 0){
                    marks[pos] = 1;
                    n_marks++;
                }
            }
            pos = (pos + 1) % q->waiting->size;
        }
        close_and_remove_with_marks(q->waiting, marks);
        if (needed_marks == 2){
            pthread_cond_signal(q->has_room);
        }else if (needed_marks > 2){ //could signal needed_marks times but why 
            pthread_cond_broadcast(q->has_room);
        }

            break;
        
        default:
            break;
        }
    }
    enqueue(q->waiting, &task);
    pthread_cond_signal(q->has_elements);
ret:
    pthread_mutex_unlock(q->lock);

}

Task* get_task(Taskq *q)
{
    pthread_mutex_lock(q->lock);
    Node *node;

    while (q->waiting->size == 0){
        pthread_cond_wait(q->has_elements, q->lock);
    }
    Task task = dequeue(q->waiting);
    _fill_dispatch(&task);
    // printf("in get_task:%lu.%06lu\n", task.dispatch.tv_sec, task.dispatch.tv_usec);

    node = enqueue(q->running, &task);
    pthread_mutex_unlock(q->lock);
    return &node->task;

}

Task* get_last_task(Taskq *q)
{
    pthread_mutex_lock(q->lock);
    Node *node;
    while (q->waiting->size == 0){
        pthread_cond_wait(q->has_elements, q->lock);
    }
    Task task = popLast(q->waiting);
    _fill_dispatch(&task);
    node = enqueue(q->running, &task);
    pthread_mutex_unlock(q->lock);
    return &node->task;
}

Task mark_done(Taskq *q, Task *task)
{
    pthread_mutex_lock(q->lock);
    Task ret_task;
    Node* node = container_of(task, Node, task);
    ret_task = removeNode(q->running, node);
    _fill_finished(&ret_task);
    pthread_cond_signal(q->has_room);
    if (q->waiting->size + q->running->size == 0)
        pthread_cond_signal(q->empty);

    pthread_mutex_unlock(q->lock);
    return ret_task;
}

void _fill_dispatch(Task* task)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    task->dispatch.tv_sec = now.tv_sec - task->arrival.tv_sec;
    task->dispatch.tv_usec = now.tv_usec - task->arrival.tv_usec;
}
void _fill_finished(Task* task)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    task->finished.tv_sec = now.tv_sec - task->arrival.tv_sec;
    task->finished.tv_usec = now.tv_usec - task->arrival.tv_usec;
}