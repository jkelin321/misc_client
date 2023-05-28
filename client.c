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

    char fileBuffer[255];

    int sockfd, portno, n;
    struct sockaddr_in serv_addr; // <-- from netinet/in.h
    struct hostent * server; // <-- from netdb.h , gethostbyname is techincally obsolete

    FILE * fp = fopen("input.txt", "r");
    if (fp == NULL)
        return 1;
    
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
    serv_addr.sin_family = AF_INET;
    memmove((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
       error("Connection Failed");

    while(1){ //TODO change if statement before n = write(..) to terminate if reutrns null / EOF. TODO strip \n off end of each file entry -- pass without \n to fpga
        memset(fileBuffer, 0, 255 );

        if (!fgets(fileBuffer, 255, fp)) // reads fp --> returns NULL if error / EOF
            return 1;
        fileBuffer[strlen(fileBuffer)-1] = '\0'; // overwrite the escape on each line to be \0 <-- don't wan't these landed on microblaze
        n = write(sockfd, fileBuffer, strlen(fileBuffer));
                                                      //    fgets(buffer, 255, stdin);  <--  "gets from file stream"
        if(n<0)
            error ("Error on writing");

        memset(fileBuffer, 0, 255); // clear buffer after writing 

        n = read(sockfd , fileBuffer , 255);
        if (n<0)    
            error("Error on reading");
        printf("Server: %s\n\r", fileBuffer);
                                    // would clear buffer again, but done at default of while loop
        int i = strncmp("Bye", fileBuffer, 3);
        if(i == 0)
            break;
    }
    
    close(sockfd);
    return 0;

}
