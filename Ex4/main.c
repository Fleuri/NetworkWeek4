/* inet.h
* Definitions for TCP and UDP client/server programs.
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAXLINE 512
#define SERV_UDP_PORT 51012
#define SERV_TCP_PORT 51012
/* host addr for server*/
#define SERV_HOST_ADDR "128.214.9.98" /* melkki.cs.helsinki.fi */

void my_err(char *sptr)
{
perror(sptr);
exit(1);
}



void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, int servlen) {
int n;
char sendline[MAXLINE], recvline[MAXLINE + 1];

    while (fgets(sendline, MAXLINE, fp) != NULL) {
            n = strlen(sendline);
            if (sendto(sockfd, sendline, n, 0, pservaddr, servlen) !=n)
                    my_err(“dg_cli:sendto error on socket”);
            n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
            if (n<0) my_err(“dg_cli: recvfrom error”);
                    recvline[n] = 0; /* null terminate */
            fputs(recvline, stdout);
    }
    if (ferror(fp)) my_err(“dg_cli:error reading file”);
    }

/*
 * 
 */
int main(int argc, char** argv) {

        int sockfd;
        struct sockaddr_in servaddr;
        if (argc != 2) {
                fprintf(stderr, "usage: udpcli <IPaddress>\n"); exit(1);
        }
       
        bzero( (void *) &servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERV_UDP_PORT);
        if ( inet_aton(argv[1], &servaddr.sin_addr) == 0) {
                fprinf(stderr, "udpcli: invalid IP address %s\n", argv[1]);
                exit(1);
        }
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0) ) < 0)
                my_err(udpcli:socket error”) ;
        dg_cli(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
        exit(0);
}



