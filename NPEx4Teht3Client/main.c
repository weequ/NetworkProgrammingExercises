/* Sample UDP client */

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
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


int main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr;
   char sendline[1000];
   //char recvline[1000];

   if (argc != 2)
   {
      printf("usage:  udpcli <IP address>\n");
      exit(1);
   }

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
   servaddr.sin_port=htons(32000);
   char buffer[256];
   int len;
   printf("started reading\n");
   while ((len = readLinee(STDIN_FILENO, buffer, 256)) > 0) {
       printf("sending data\n");
       sendto(sockfd, buffer, len, 0, (struct sockaddr *)&servaddr,sizeof(servaddr));
   }
}
