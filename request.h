#ifndef __REQUEST_H__
#define __REQUEST_H__

typedef struct 
{
   int fd;
   char buf[MAXLINE];
   char method[MAXLINE];
   char uri[MAXLINE];
   char version[MAXLINE];
   rio_t rio
} request;

typedef struct Threads_stats{
	int id;
	int stat_req;
	int dynm_req;
	int total_req;
} * threads_stats;

void requestHandle(request *req);

void reqInit (request *req,int fd);
int isSkip(const char *str );

#endif
