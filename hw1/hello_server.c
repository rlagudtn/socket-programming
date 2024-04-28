#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h> //네트워크 통신을 위한 소켓 인터페이스, 자료형, 구조체
void error_handling(char * message);

int main(int argc, char * argv[]){
  //소켓 통신을 위한 구조체 및 변수
  int serv_sock;
  int clnt_sock;
  struct sockaddr_in serv_addr; 
  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size;


  char message[]="Hello World\n";
  if (argc!=2){
    printf("Usage : %s <port>\n",argv[0]);
    exit(1);
  }

  // 1. 소켓 생성. serv_sockt은 관리를 위해 사용하며, clnt_serv 가 실질적인 통신을 담당
  serv_sock=socket(PF_INET, SOCK_STREAM,0);

  if(serv_sock==-1)
    error_handling("socket() error");
  
  memset(&serv_addr,0,sizeof(serv_addr));
  // 2. 소켓 초기화
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_addr.sin_port=htons(atoi(argv[1]));
  if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
    error_handling("bind() error");

  // 3. 소켓 
  if(listen(serv_sock,5)==-1)
    error_handling("listen() error");
  clnt_addr_size=sizeof(clnt_addr);
  clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
  if(clnt_sock==-1)
    error_handling("accept() error");

  // message 출력
  write(clnt_sock,message,sizeof(message));
  close(clnt_sock);
  close(serv_sock);

  return 0;
}

void error_handling(char * message){
  fputs(message,stderr);
  fputc('\n',stderr);
  exit(1);
}