#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <pigpio.h>

#define PORTNUM 0x9190
#define BUFSIZE 256

#define pin 18

int main(){
    struct sockaddr_in serv_adr,clnt_adr;
    int serv_fd, clnt_fd, clnt_len;
    char buffer[BUFSIZE];

    printf("[TCP server for chatting and controlling LED ...]\n");

    if(gpioInitialise()==-1)
        return 1;
    
    gpioSetMode(pin,PI_OUTPUT);

    // create server socker
    if((serv_fd=socket(PF_INET,SOCK_STREAM,0))==-1){
        perror("error socket()");
        exit(1);
    }

    memset((char*)&serv_adr,0x00,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(PORTNUM);

    if(bind(serv_fd,(struct sockaddr *)&serv_adr,sizeof(serv_adr))==-1){
        perror("error bind()");
        exit(1);
    }

    listen(serv_fd,5);

loop:
    clnt_len=sizeof(clnt_adr);
    if((clnt_fd=accept(serv_fd,(struct sockaddr *)&clnt_adr,&clnt_len))==-1){
        perror("error accept");
        exit(1);
    }

    write(clnt_fd,"Welcome to chat server ......LED control..",BUFSIZE);
    while(1){
        memset(buffer,0x00,sizeof(buffer));
        if((read(clnt_fd,buffer,BUFSIZE))==-1){
            perror("error read()");
            exit(1);
        }
        printf("[Guest] %s\n",buffer);

        if(buffer[0]=='q'){
            close(clnt_fd);
            goto loop;
        }
        else if(buffer[0]=='0')
            gpioWrite(pin,0);
        else if(buffer[0]=='1')
            gpioWrite(pin,1);

        memset(buffer,0x00,sizeof(buffer));
        printf("[Server] Enter key(s):");
        fgets(buffer,BUFSIZE,stdin);
        write(clnt_fd,buffer,BUFSIZE);
        if(buffer[0]=='q')
            break;
        
    }
    
    close(serv_fd);
    gpioTerminate();

    return 0;

}