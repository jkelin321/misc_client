#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char * msg){
    perror(msg);
    exit(0);
}

int main (int argc, char * argv[]){

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent * server;

    char buffer[256];
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port \n", argv[0]);
        exit (1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if(server == NULL)
        fprintf(stderr , "Error , no such host");

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
//    bzero((char *)&serv_addr, sizeof(serv_addr));    //         <-- swapped for troubleshooting
    serv_addr.sin_family = AF_INET;
    memmove((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
//    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); //<-- swapped for troubleshooting
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
        error("Connection Failed");

    while(1){
        memset(buffer, 0, 255);
//        bzero(buffer, 255);//       <-- swapped for troubleshooting
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if(n<0)
            error ("Error on writing");

        memset(buffer, 0, 255);
//        bzero(buffer, 255);//       <-- swapped for troubleshooting
        n = read(sockfd , buffer , 255);
        if (n<0)    
            error("Error on reading");
        printf("Server: %s", buffer);
        
        int i = strncmp("Bye", buffer, 3);
        if(i == 0)
            break;
    }
    
    close(sockfd);
    return 0;

}
