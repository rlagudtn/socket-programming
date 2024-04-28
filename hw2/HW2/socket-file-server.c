#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1096
void error_handling(char *message);

int main(int argc,char *argv[]){
  int serv_sock,clnt_sock;
  char message[BUF_SIZE];

  struct sockaddr_in serv_adr,clnt_adr;
  socklen_t clnt_adr_sz;
  int str_len;
  if(argc!=2){
    printf("Usage %s <port>\n",argv[0]);
    exit(1);
  }

  serv_sock=socket(PF_INET,SOCK_STREAM,0);
  if(serv_sock==-1)
    error_handling("socket() error");
  
  memset(&serv_adr,0,sizeof(serv_sock));
  serv_adr.sin_family=AF_INET;
  serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_adr.sin_port=htons(atoi(argv[1]));

  if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
    error_handling("bind() error");

  if(listen(serv_sock,5)==-1)
    error_handling("listen() error");
  
  clnt_adr_sz=sizeof(clnt_adr);
  clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
  if(clnt_sock==-1)
    error_handling("accept() error");
  else
    printf("Connected client \n");
  
  //파일 이름 받기
  if(read(clnt_sock,message,sizeof(message)-1)==-1)
    printf("cannot read message\n");
  printf("file name >>>> %s\n",message);

  // 파일 오픈
  FILE *fp;
  if((fp=fopen(message,"wb"))==NULL){
    printf("cannot open file\n");
    exit(1);
  }

  //데이터 읽기 및 파일 저장
  while((str_len=read(clnt_sock,message,BUF_SIZE))>0)
    fwrite(message,sizeof(char),str_len,fp);
  printf("file save complete\n");


  fclose(fp);

  close(clnt_sock);
  close(serv_sock);

  return 0;
}

void error_handling(char * message){
  fputs(message,stderr);
  fputc('\n',stderr);
  exit(1);
}