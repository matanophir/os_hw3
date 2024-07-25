#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "segel.h"

Node* createNode(Task *task)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory error\n");
        return NULL;
    }
    newNode->task.fd = task->fd;
    newNode->task.arrival = task->arrival;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;

}

// Function to create a queue
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        printf("Memory error\n");
        return NULL;
    }
    queue->front = queue->rear = NULL;
    queue->size = 0;
    return queue;
}

Node* enqueue(Queue* queue, Task *task)
{
    Node* newNode = createNode(task);
    if (!newNode) return NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        newNode->prev = queue->rear;
        queue->rear = newNode;
    }
    queue->size++;
    return newNode;
}

Task dequeue(Queue* queue)
{
    if (queue->front == NULL) {
        printf("Queue is empty\n");
        Task t = {-1, {0,0}};
        return t;
    }
    Node* temp = queue->front;
    Task task = temp->task;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    } else {
        queue->front->prev = NULL;
    }
    free(temp);
    queue->size--;
    return task;

}

int size(Queue* queue)
{
    return queue->size;
}

//remove from the middle of the list
Task removeNode(Queue* queue, Node* node)
{
    Task task = {-1, {0, 0}};
    if (queue->front == NULL || node == NULL) return task;
    if (node == queue->front) {
        return dequeue(queue);
    }
    task = node->task;
    if (node == queue->rear) {
        queue->rear = node->prev;
        queue->rear->next = NULL;
        free(node);
        queue->size--;
        return task;
    }
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
    queue->size--;
    return task;

}

Task popLast(Queue* queue)
{
    Task task = {-1, {0, 0}};
    if (queue->rear == NULL) {
        printf("Queue is empty\n");
        return task;
    }
    Node* temp = queue->rear;
    task = temp->task;
    queue->rear = queue->rear->prev;
    if (queue->rear == NULL) {
        queue->front = NULL;
    } else {
        queue->rear->next = NULL;
    }
    free(temp);
    queue->size--;
    return task;
}


// Function to print the queue
void printQueue(Queue* queue) {
    Node* temp = queue->front;
    while (temp != NULL) {
        printf("%d -> ", temp->task.fd);
        temp = temp->next;
    }
    printf("NULL\n");
}

// // Example usage
// int main() {
//     Queue* queue = createQueue();
//     Task* task1 = enqueue(queue, 10);
//     Task* task2 = enqueue(queue, 20);
//     Task* task3 = enqueue(queue, 30);
//     printf("Queue: ");
//     printQueue(queue);
    
//     printf("Dequeued: %d\n", dequeue(queue));
//     printf("Queue: ");
//     printQueue(queue);
    
//     removeTask(queue, task2);
//     printf("Queue after removing middle task: ");
//     printQueue(queue);
    
//     printf("Size of the queue: %d\n", size(queue));
//     return 0;
// }
