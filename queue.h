#ifndef QUEUE_H
#define QUEUE_H

#include "segel.h"

// Define the structure for a task
typedef struct Task {
    int data;
    struct timeval* arrival; //holds pointer to time
    struct timeval _arrival; //holds the time
    struct Task* next;
    struct Task* prev;
} Task;

// Define the structure for a queue
typedef struct Queue {
    Task* front;
    Task* rear;
    int size;
} Queue;

// Function to create a new task
Task* createTask(int data, struct timeval *arrival) ;

// Function to create a queue
Queue* createQueue();

// Function to enqueue an element to the queue
Task* enqueue(Queue* queue, int data, struct timeval *arrival);

// Function to dequeue an element from the queue
int dequeue(Queue* queue);

// Function to get the size of the queue
int size(Queue* queue);

// Function to remove a task from the middle of the queue
void removeTask(Queue* queue, Task* task);

// Function to pop the last element from the queue
int popLast(Queue* queue);

// Function to print the queue
void printQueue(Queue* queue);

#endif // QUEUE_H
