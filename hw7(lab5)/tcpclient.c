#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define PORTNUM 0x9190
#define BUFSIZE 256

int main(int argc,char * argv[]){
    int sock_fd;
    struct sockaddr_in serv_adr;
    char buffer[BUFSIZE];

    printf("[TCP server  for chatting LED...]\n");

    if((sock_fd=socket(PF_INET,SOCK_STREAM,0))==-1){
        perror("error sock()");
        exit(1);
    }

    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(PORTNUM);

    if(connect(sock_fd,(struct sockaddr *)&serv_adr,sizeof(serv_adr))==-1){
        perror("error conneting");
        exit(1);
        
    }

    memset(buffer,0x00,sizeof(buffer));
    if(read(sock_fd,buffer,BUFSIZE)==-1){
        perror("error reading");
        exit(1);

    }
    printf("[Server]%s\n",buffer);
    while(1){
        memset(buffer,0x00,sizeof(buffer));

        printf("[Guest] Enter key(s):");
        fgets(buffer,BUFSIZE,stdin);

        if(write(sock_fd,buffer,strlen(buffer))==-1){
            perror("error writing");
            exit(1);
        }
        if(buffer[0]=='q')
            break;
        
        memset(buffer,0x00,sizeof(buffer));
        if(read(sock_fd,buffer,BUFSIZE)==-1){
            perror("error read");
            exit(1);

        }
        printf("[Server]%s\n",buffer);
        if(buffer[0]=='q')
            break;
        
    }
    close(sock_fd);
    return 0;
}