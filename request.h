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
}request;

void requestHandle(request *req);

void reqInit (request *req,int fd);

#endif
