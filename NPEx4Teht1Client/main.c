/* 
 * File:   main.c
 * Author: antti
 *
 * Created on February 4, 2015, 2:31 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))



/*
 * 
 */
int main(int argc, char** argv) {
    int sockfd,n, len;
    struct sockaddr_in servaddr,cliaddr;
    char sendline[1000];
    char recvline[1000];

    if (argc != 4)
    {
       printf("usage:  client <IP address> <TotalWrite> <WriteOnce>\n");
       exit(1);
    }
    int totalwrite = atoi(argv[2]);
    int writeonce = atoi(argv[3]);
    sockfd=socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(argv[1]);
    servaddr.sin_port=htons(50001);

    
    struct timeval start, end;
    gettimeofday(&start, NULL);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("error connecting");
        exit(1);
    }
    gettimeofday(&end, NULL);
    printf("Establishing connection took: %d ms.\n", ((end.tv_sec*1000 + end.tv_usec/1000)-(start.tv_sec*1000 + start.tv_usec/1000)));
    int count;
    char line[writeonce];
    for (count = 0; count < writeonce; count++) {
        line[count] = 'a';
    }
    int byteswritten = 0;
    
    gettimeofday(&start, NULL);
    while(byteswritten < totalwrite) {
        byteswritten += write(sockfd, line, MIN(writeonce, totalwrite-byteswritten));
    }
    gettimeofday(&end, NULL);
    printf("Time passed writing %d bytes: %d ms.\n", byteswritten, ((end.tv_sec*1000 + end.tv_usec/1000)-(start.tv_sec*1000 + start.tv_usec/1000)));
    close(sockfd);
    sleep(1);
    return (EXIT_SUCCESS);
}

