/* 
 * File:   main.c
 * Author: antti
 *
 * Created on February 4, 2015, 2:30 PM
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

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
    int buffersize = atoi(argv[1]);
    char buffer[buffersize];
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
            close(sockfd); /* close original socket */
            printf("waiting for the client to send text:\n");
            struct timeval start, end;
            gettimeofday(&start, NULL);
            while ((n = read(newsockfd, buffer, buffersize)) > 0) {
                //write(STDOUT_FILENO, buffer, n);
            }
            gettimeofday(&end, NULL);
            printf("Reading data took: %d ms.\n", ((end.tv_sec*1000 + end.tv_usec)-(start.tv_sec*1000 + start.tv_usec)));
            printf("client closed.\n");
            close(newsockfd);
            exit(0);
        }
        close(newsockfd); /* parent process */
    }
    return (EXIT_SUCCESS);
}

