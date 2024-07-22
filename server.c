#include "segel.h"
#include "request.h"

#include "taskq.h"


// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// HW3: Parse the new arguments too
void getargs(int *port,int *threads,int *queue_size,char* schedlag, int argc, char *argv[])
{
    if (argc < 5) {
	fprintf(stderr, "Usage: %s <port> <threads> <queue_size> <schedlag>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *threads = atoi(argv[2]);
    *queue_size = atoi(argv[3]);
    schedlag = malloc(strlen(argv[4])+1);
    strcpy(schedlag, argv[4]);
    if (*threads < 1 || *queue_size < 1){
        fprintf(stderr, "threads and queue size must be a positive integer\n"); //TODO should check if double?
        exit(1);
    }
    if (strcmp(schedlag, "block") != 0 && strcmp(schedlag, "dt") != 0 && strcmp(schedlag, "dh") != 0 && strcmp(schedlag, "bf") != 0 && strcmp(schedlag, "random") != 0) {
    fprintf(stderr, "schedalg doesn't exist\n");
    exit(1);
    }
}
void* worker_func(void* queue)
{
    Task* task;
    request req;
    int ret_code;
    Taskq* q = (Taskq*)queue; //for debugging 
    while (1)
    {
        task = get_task(q);
        reqInit(&req, task->data); // a request can handled once for now. cause socket read.
        // requestHandle(&req);
        // Close(task->data);
        // mark_done(q, task);
        // continue;

        if (isSkip(req.uri))
        {
            Task* skip_task = get_last_task(q);
            request req_skip;
            reqInit(&req_skip, skip_task->data);
            requestHandle(&req);
            Close(task->data);
            mark_done(q, task);

            requestHandle(&req_skip);
            Close(skip_task->data);
            mark_done(q, skip_task);
            continue;
        }else
        {
            requestHandle(&req);
            Close(task->data);
            mark_done(q, task);
        }
        

    }
    
}


int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    int threads, queue_size;
    char *schedlag;
    struct sockaddr_in clientaddr;

    getargs(&port,&threads, &queue_size, schedlag, argc, argv);

    Taskq *task_q = createTaskq(queue_size);
    pthread_t *threads_arr = malloc(sizeof(pthread_t)*threads);

    // 
    // HW3: Create some threads...
    //
    for (int i = 0; i < threads; i++)
    {
        pthread_create(&threads_arr[i],NULL, worker_func, (void*)task_q);
    }

    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

	// 
	// HW3: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. 
	// 
    add_task(task_q,connfd);

	// requestHandle(connfd);
	// Close(connfd);
    }

}


    


 
