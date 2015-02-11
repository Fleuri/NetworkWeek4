#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define SERV_UDP_PORT 51012
#define SERV_TCP_PORT 51012
#define SERV_HOST_ADDR "12.0.0.1"
 
void my_err(char *sptr) {
    perror(sptr);
    exit(1);
}
 
void echo_double(int sockfd, struct sockaddr *pcliaddr, int clilen){
    int n;
    int len;
    char mesg[128];
   
    while(1){
        len = clilen;
        n = recvfrom(sockfd, mesg, 128, 0, pcliaddr, &len);
        if (n < 0 ){
            my_err("Doubler ;__; \n");
        }
        fputs(mesg, stdout);
        fputs(mesg, stdout);
        memset(&mesg, 0, sizeof(mesg));
//        if (sendto(sockfd, mesg, n, 0, pcliaddr, len) != n){
//            my_err("Doubler send error ;__; \n");
//        }
    }
}
 
/*
 *
 */
int main(int argc, char** argv) {
 
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
 
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, PF_UNSPEC)) < 0) {
        my_err("Server: Can\'t create socket\n");
    }
 
    bzero((void *) &servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_UDP_PORT);
 
    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0) {
        my_err("Server: can\'t bind datagram socket\n");
    }
    echo_double(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
 
}