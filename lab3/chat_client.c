#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 256
#define NAME_SIZE 32

void error_handling(char *message);
void* send_msg(void *sock);
void* recv_msg(void *sock);

char msg[BUF_SIZE]="[DeFAULT]";
char name[NAME_SIZE];
int main(int argc,char * argv[]){
    int sock;
    struct sockaddr_in sock_adr;
    void * thread_return;
    if(argc!=4){
        printf("Usage : %s <IP> <port> <name>\n",argv[0]);
        exit(1);
    }
    sprintf(name , "[%s]",argv[3]);
    if((sock=socket(PF_INET,SOCK_STREAM,0))==-1)
        error_handling("socket() error");
         
    sock_adr.sin_family=AF_INET;
    sock_adr.sin_addr.s_addr=inet_addr(argv[1]);
    sock_adr.sin_port=htons(atoi(argv[2]));
    if(connect(sock,(struct sockaddr *)&sock_adr,sizeof(sock_adr))==-1)
        error_handling("connect() error");
    
    pthread_t send_thread,recv_thread;
    pthread_create(&send_thread,NULL,send_msg,(void *)&sock);
    pthread_create(&recv_thread,NULL,recv_msg,(void*)&sock);
    pthread_join(send_thread,&thread_return);
    pthread_join(recv_thread,&thread_return);

    close(sock);

    return 0;
}

void* send_msg(void *sock){
    int send_sock=*((int*)sock);

    char name_msg[NAME_SIZE*BUF_SIZE];
    while(1){
        fgets(msg,BUF_SIZE,stdin);
        if(!strcmp(msg,"q\n") ||!strcmp(msg,"Q\n")){
            close(send_sock);
            exit(0);
        }
        sprintf(name_msg,"%s %s",name,msg);
        write(send_sock,name_msg,strlen(name_msg));
    }
    return NULL;

}
void* recv_msg(void *sock){
    int recv_sock=*((int *)sock);
    char name_msg[NAME_SIZE*BUF_SIZE];
    int str_len;
    while(1){
        if((str_len=read(recv_sock,name_msg,NAME_SIZE*BUF_SIZE))==-1)
            return (void*)(-1);
        name_msg[str_len]='0';
        fputs(name_msg,stdout);
    }
    return NULL;
}
void error_handling(char *message){
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}