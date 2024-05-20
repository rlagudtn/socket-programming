#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char * message);

int main(int argc, char *argv[]){
  int serv_sock,clnt_sock;
  char message[BUF_SIZE];
  int str_len,i;

  struct sockaddr_in serv_adr;
  struct sockaddr_in clnt_adr;
  socklen_t clnt_adr_sz;

  if (argc!=2){
    printf("Usage: %s <port>\n",argv[0]);
    exit(1);
  }

  // tcp 소켓 생성
  // socket 매개변수가 의미하는 것은 뭔가?
  // socket(도메인,타입, 프로토콜)
  // 도메인: 소켓에 사용할 프로토콜 체계(프로코톨의 종류) | PF_INET: IPv4, PF_INET6:IPv6, ...
  // 타입: 소켓 데이터 전송 방식 -> tcp, udp | SOCK_STREAM(연결지향, TCP), SOCK_DGRAM(비연결지향, UDP)
  // 프로토콜: 두 컴퓨터간 통신에 사용하는 프로토콜 | 1,2인자로 결정되기 때문에 0 적어도 상관 x
  serv_sock=socket(PF_INET,SOCK_STREAM,0);
  if (serv_sock==-1)
    error_handling("socket() error");
  
  //serv_sock 메모리 할당
  // 이후, serv_adr의 구조체 초기화
  // sin_familiy, sin_addr, sin_port 가 의미하는 것과 값이 의미하는 건?
  memset(&serv_adr,0, sizeof(serv_adr));
  serv_adr.sin_family=AF_INET;
  serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_adr.sin_port=htons(atoi(argv[1]));
  //바이트 변환함수
  // htons, htonl >> h(host), n(network),s(short), l(long) 
  // h: 호스트 바이트 순서 의미, n: 네트워크 바이트 순서 의미,
  // s: 자료형 short 의미, l: 자료형 long 의미


  // bind 함수 실행
  // bind(소켓, sockaddr 자료구조, sockaddr 크기) 
  if(bind(serv_sock,(struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
    error_handling("bind() error");

  //listen 함수 실행
  // listen(소켓, backlog )
  // backlog: 연결 요청 대기 큐의 크기
  if(listen(serv_sock,5)==-1)
    error_handling("listen() error");
  
  clnt_adr_sz=sizeof(clnt_adr);

  // clnt_sock을 생성해서 read/write 수행하고 close를 반복
  for(int i=0;i <5;i++){
    //clnt sock= accept(서버 소켓, 클라이언트 소켓 adr, 사이즈)
    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    if(clnt_sock==-1)
      error_handling("accept() error");
    else 
      printf("Connect client: %d \n",i+1);
    while((str_len=read(clnt_sock,message,BUF_SIZE))!=0)
      write(clnt_sock,message,str_len);

    close(clnt_sock);
  }
  close(serv_sock);
  return 0;
}

void error_handling(char * message){
  fputs(message,stderr);
  fputc('\n',stderr);
  exit(1);
}