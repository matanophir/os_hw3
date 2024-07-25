#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "taskq.h"

typedef struct 
{
   int fd;
   char buf[MAXLINE];
   char method[MAXLINE];
   char uri[MAXLINE];
   char version[MAXLINE];
   rio_t rio;
   struct timeval arrival;
   struct timeval dispatch;
} request;

typedef struct Threads_stats{
	int id;
	int stat_req;
	int dynm_req;
	int total_req;
}  threads_stats;

void requestHandle(request *req, threads_stats* t_stats);

void reqInit (request *req,Task *task);
int isSkip(const char *str );

#endif
