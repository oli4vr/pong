/* pong.c
 * Simple port scan tool
 * for network connection testing (opposed to icmp ping)
 * by Olivier Van Rompuy
 *
 * CSV formatted output as "hostname;port;error;error_description"
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
#include <errno.h>
#include "errnostr.h"

#define BUF_SIZE 1500

int pong(unsigned char * hostname, int port, int tout) {
 int s,n,rc,e;
 struct sockaddr_in sin;
 struct hostent *host;
 struct in_addr **addr_list;
 char buf[BUF_SIZE]={0};

 struct timeval timeout;      
 timeout.tv_sec = tout;
 timeout.tv_usec = 0;

 s=socket(AF_INET,SOCK_STREAM,0);
 e=errno;
 if (s<0) return e;

 host = gethostbyname(hostname);
 if (host == NULL) {
  return -1;
 }

 sin.sin_addr = *((struct in_addr*) host->h_addr);
 sin.sin_family = AF_INET;
 sin.sin_port = htons(port);

 rc=setsockopt (s, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout));
 e=errno;
 if (rc<0) {
  close(s);
  return e;
 }

 if (connect(s ,(struct sockaddr *)&sin,sizeof(sin))<0)
 {
  e=errno;
  close(s);
  return e;
 }

 close(s);
 return 0;
}

int string2ports(unsigned char *str, unsigned int *ports) {
 unsigned char buff[256]={0};
 unsigned char * sp=buff;
 unsigned char * sw=buff;
 unsigned char n, sz, nr=0;
 strncpy(buff,str,255);
 sz=strnlen(buff,255)+1;
 for(n=0;n<sz;n++) {
  if(*sp==0 || *sp==',') {
    *sp=0;
    nr++;
    *ports=atoi(sw);
    ports++;
    sw=sp+1;
  } else if (*sp<'0' || *sp>'9') {
    return -1;
  }
  sp++;
 }
 return nr;
}

int pong_ports(unsigned char * hostname,unsigned char *portstr, int timeout) {
 unsigned int ports[256];
 unsigned char nrports=0,n;
 int rc;
 nrports=string2ports(portstr,ports);
 if (nrports<0) return -1;  //Syntax error
 for(n=0;n<nrports;n++) {
    rc=pong(hostname,ports[n],timeout);
    if (rc<0) fprintf(stdout,"%s;%d;ERRGETHOST;Hostname could not be resolved\n",hostname,ports[n]);
    else fprintf(stdout,"%s;%d;%s;%s\n",hostname,ports[n],errno_string(rc),errno_description(rc));
 }
 return nrports;
}

void helptext(unsigned char * cmd) {
 fprintf(stderr,"%s\n\t-> Simple port scan tool for network connection testing\n",cmd);
 fprintf(stderr,"\t-> by Olivier Van Rompuy\n");
 fprintf(stderr,"Syntax     :\n\t%s HOSTNAME|IP [PORT1,PORT2,PORT3,...] [TIMEOUT]\n",cmd);
 fprintf(stderr,"Examples   :\n\t%s 127.0.0.1\n\t%s 127.0.0.1 22\n\t%s 127.0.0.1 22,80,443\n",cmd,cmd,cmd);
 fprintf(stderr,"CSV Output :\n\tHOST;PORT;ERRNO;ERRNO_DESCRIPTION\n\n");
}


int main(int argc,char ** argv)
{
 unsigned char ports[256]="22";
 unsigned char hostname[64]={0};
 unsigned char badsyntax=0,rc;
 unsigned char timeout=2;
 if (argc<2) {helptext(argv[0]);return -1;}
 strncpy(hostname,argv[1],63);
 if (argc>2) {strncpy(ports,argv[2],255);}
 if (argc>3) {timeout=atoi(argv[3]);}
 rc=pong_ports(hostname,ports,timeout);
 if (rc<0) {helptext(argv[0]);return -2;}
 return 0;
}

