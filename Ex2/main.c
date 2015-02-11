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
 
void client_send(FILE *fp, int sockfd, const struct sockaddr *pservaddr, int servlen){
    int n;
    char sendline[128], recvline[129];
   
    while(fgets(sendline, 128, fp) != NULL){
        n = strlen(sendline);
        if(sendto(sockfd, sendline, n, 0, pservaddr, servlen) != n){
            my_err("Client_send: Error socket\n");
        }
//        n = recvfrom(sockfd, recvline, 128, 0, NULL, NULL);
//        if(n<0){
//            my_err("client_send: recvfrom erro\n");
//        }
//        recvline[n] = 0;
//        fputs(recvline, stdout);
    }
    if(ferror(fp)){
        my_err("client_send: error reading file\n");
    }
}
/*
 *
 */
int main(int argc, char** argv) {
   
    int sockfd;
    struct sockaddr_in servaddr;
   
    if(argc != 2){
        fprintf(stderr, "usage: prog <IP-address>\n");
        exit(1);
    }
   
    bzero((void *) &servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_UDP_PORT);
    if (inet_aton(argv[1], &servaddr.sin_addr) == 0){
        fprintf(stderr, "Client: invalid IP-address %s\n", argv[1]);
        exit(1);
    }
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0) ) < 0){
        my_err("Client: Socket error \n");
    }
   
    client_send(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    exit(0);
 
}