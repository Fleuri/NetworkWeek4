#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/types.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
 
 
int readLinee(int fd, char* buf, int maxlen) {
    char c;
    ssize_t count;
    int i;
    for (i = 0; i < maxlen; i++) {
        count = read(fd, &c, 1);
        if (count < 0) {
            perror("read < 0\n");
            exit(1);
        } else if (count == 0) {
            break;
        }
        if (c == '\n') {
            break;
        }
        buf[i] = c;
    }
    return i;
}
 
/*
 *
 */
int main(int argc, char** argv) {
    int sockfd,n, len;
    struct sockaddr_in servaddr,cliaddr;
    char sendline[1000];
    char recvline[1000];
    int dataLength = atoi(argv[2]);
    int blockLength = atoi(argv[3]);
    struct addrinfo *result;
 
    if (argc != 2)
    {
       printf("usage:  client <IP address>\n");
       exit(1);
    }

    getaddrinfo("ukko135.hpc.cs.helsinki.fi", "echo", NULL, &result);
    servaddr = (struct sockaddr_in) result->ai_addr;
 
    sockfd=socket(AF_INET,SOCK_STREAM,0);
 
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(argv[1]);
    servaddr.sin_port=htons(50001);
 
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("error connecting");
        exit(1);
    }
    
    while ((len = readLinee(STDIN_FILENO, sendline, 1000)) > 0) 
        write(sockfd, sendline, len);
        write(sockfd, "\n", 1);
    return (EXIT_SUCCESS);
}