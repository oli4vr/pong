/* pong.c
 *
 * by Olivier Van Rompuy
 *
 * Simple TCP single port scan tool.
 * Test whether a specific tcp port on a host is accessible.
 *
 * Syntax : pong hostname port
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <inttypes.h>
#include <time.h>

#define BUF_SIZE 1400


int main(int argc,char ** argv)
{
 int s,n;
 struct sockaddr_in sin;
 struct hostent *host;
 struct in_addr **addr_list;
 char buf[BUF_SIZE]={0};

 struct timeval timeout;      
 timeout.tv_sec = 2;
 timeout.tv_usec = 0;

 if (argc<3) {
	 printf("pong\nTCP SINGLE PORT SCAN TOOL\nby Olivier Van Rompuy\n\nSyntax : %s host port\n\n",argv[0]);
	 return -1;
 }

 s=socket(AF_INET,SOCK_STREAM,0);
 if (s==-1) return -2;

 host = gethostbyname(argv[1]);
 if (host == NULL) {
  printf("Hostname_Lookup_Failed\n");
  return -10;
 }

 sin.sin_addr = *((struct in_addr*) host->h_addr);
 sin.sin_family = AF_INET;
 sin.sin_port = htons( atoi(argv[2]) );

 setsockopt (s, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout));

 if (connect(s ,(struct sockaddr *)&sin,sizeof(sin))<0)
 {
  printf("Connect_Failed\n");
  return -3;
 }

 printf ("Success\n");

 close(s);

 return 0;
}

