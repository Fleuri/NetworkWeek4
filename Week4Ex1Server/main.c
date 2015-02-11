 
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
 
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
    struct sockaddr_in serv_addr, cli_addr;
    int portno;
    int sockfd;
    int clilen;
    int newsockfd;
    int n;
    int readLength = atoi(argv[1]);
    char buffer[255];
    struct timeval start, end;
    int timestarted = 0;

    sockfd = socket(PF_INET, SOCK_STREAM, PF_UNSPEC);
    if (sockfd < 0) {
        perror("Errror creating stream socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 50001;
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portno);
   
   
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }
    listen(sockfd, 5);
    while(1) {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0){
            perror("ERROR on accept");
            exit(1);
        }
        int childpid;
        if ( (childpid = fork()) < 0)
            perror("Can't fork");
        else if (childpid == 0) { /* child process */
        gettimeofday(&start, NULL);
            close(sockfd); /* close original socket */
            printf("waiting for the client to send text:\n");
            while ((n = read(newsockfd, buffer, readLength)) > 0) {
                write(STDOUT_FILENO, buffer, n);
                write(STDOUT_FILENO, "\n", 1);
            }
            gettimeofday(&end, NULL);
            printf("Time elapsed: %ld\n", (end.tv_sec*1000 + end.tv_usec)-(end.tv_sec*1000+start.tv_usec));
            exit(0);
        }
        close(newsockfd); /* parent process */
    }
    return (EXIT_SUCCESS);
}

