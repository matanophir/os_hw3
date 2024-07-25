#ifndef QUEUE_H
#define QUEUE_H

#include "segel.h"

typedef struct Task {
    int fd;
    struct timeval arrival; //holds the time
} Task;

// Define the structure for a node
typedef struct Node {
    Task task;
    struct Node* next;
    struct Node* prev;
} Node;

// Define the structure for a queue
typedef struct Queue {
    Node* front;
    Node* rear;
    int size;
} Queue;

// Function to create a new node
Node* createNode(Task *task) ;

// Function to create a queue
Queue* createQueue();

// Function to enqueue an element to the queue
Node* enqueue(Queue* queue, Task *task);

// Function to dequeue an element from the queue
Task dequeue(Queue* queue);

// Function to get the size of the queue
int size(Queue* queue);

// Function to remove a task from the middle of the queue
Task removeNode(Queue* queue, Node* task);

// Function to pop the last element from the queue
Task popLast(Queue* queue);

// Function to print the queue
void printQueue(Queue* queue);

#endif // QUEUE_H
