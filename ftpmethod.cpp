#include <iostream>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

#define SERVERPORT 21

int cliopen(char *host,int port){
    int control_sock;
    struct honstent *ht = NULL;
    control_sock = socket(AF_INET,SOCK_STREAM,0);
    if(control_sock<0){
       printf("socket error\n");
       return -1;
    }
    ht = gethostbyname(host);
    if(!ht){
        return -1;
    }
}
