#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1096
void error_handling(char *message);

int main(int argc,char *argv[]){
  int sock;
  char message[BUF_SIZE];
  int str_len;
  struct sockaddr_in serv_adr;

  if(argc!=4){
    printf("Usage %s <IP> <port>\n",argv[0]);
    exit(1);
  }

  sock=socket(PF_INET,SOCK_STREAM,0);
  if(sock==-1)
    error_handling("socket() error");
  
  memset(&serv_adr,0,sizeof(serv_adr));
  serv_adr.sin_family=AF_INET;
  serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
  serv_adr.sin_port=htons(atoi(argv[2]));

  if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
    error_handling("connect() error");
  else
    puts("Connected...........");

  //파일 이름 전송
  if(write(sock,argv[3],strlen(argv[3]))==-1){
    printf("cannot send filename\n");
    exit(1);
  }

  FILE *fp; 
  if((fp=fopen(argv[3],"rb"))==NULL){
    printf("cannot open file\n");
    exit(1);
  }

  // send data & close file
  while((str_len=fread(message,sizeof(char),BUF_SIZE,fp))>0)
    write(sock,message,str_len);
  
  printf("file send complete\n");
  
  fclose(fp);
  close(sock);

  return 0;
}

void error_handling(char * message){
  fputs(message,stderr);
  fputc('\n',stderr);
  exit(1);
}