#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
//추가
#include <netinet/in.h>
#include <sys/types.h>

#define MAXLINE 1024
void error_handling(char *message);

int main(int argc, char * argv[]){
  int send_sock, recv_sock; // 송수신 소켓
  struct sockaddr_in mcast_group; // 멀티캐스트 그룹 주소
  int pid;
  unsigned int yes=1;
  struct ip_mreq mreq; //멀티캐스트 그룹 구조체
  char line[MAXLINE]; 
  char name[10]; // 채팅 참가자 이름
  int n;
  socklen_t len;

  if (argc!=4){
    printf("사용법: %s multicast_address port my_name \n",argv[0]);
    exit(0);
  }
  sprintf(name,"[%s]",argv[3]);

  //멀티캐스트 수신용 소켓 생성
  if((recv_sock=socket(PF_INET,SOCK_DGRAM,0))==-1)
    error_handling("socket() error");

  memset(&mcast_group,0,sizeof(mcast_group));
  mcast_group.sin_family=AF_INET;
  mcast_group.sin_addr.s_addr=inet_addr(argv[1]);
  mcast_group.sin_port=htons(atoi(argv[2]));
  
  //소켓 옵션 설정 (멀티캐스트 그룹 & 소켓 재사용 옵션)

  //멀티 캐스트 그룹 가입
  mreq.imr_multiaddr=mcast_group.sin_addr;
  mreq.imr_interface.s_addr=htonl(INADDR_ANY);
  if(setsockopt(recv_sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))==-1)
    error_handling("error: add membership\n");

  //소켓 재사용 옵션 지정
  if(setsockopt(recv_sock,SOL_SOCKET,SO_REUSEADDR, &yes,sizeof(yes))==-1)
    error_handling("error: reuse setsockopt");

  
  //소켓 바인드
  if(bind(recv_sock,(struct sockaddr *)&mcast_group,sizeof(mcast_group))==-1)
    error_handling("bind() error");

  //멀티캐스트 메시지 송신용 소켓 생성
  if((send_sock=socket(PF_INET,SOCK_DGRAM,0))==-1)
    error_handling("error :Can't generate send socket");
  
  //fork 실행
  //child: 수신 | parent : 송신
  if((pid=fork())<0)
    error_handling("error: fork\n");
  
  else if(pid==0){ // child: 메시지 수신 담당
    struct sockaddr_in from;
    char message[MAXLINE+1];
    for(;;){
      printf("receiving message...\n");
      len=sizeof(from);
      if((n=recvfrom(recv_sock,message,MAXLINE,0,(struct sockaddr*)&from, &len))==-1)
        error_handling("error: recvfrom\n");
      message[n]=0;
      printf("Received message: %s\n",message);
    }
  }
  else{ // parent: 메시지 송신 담당
    char message[MAXLINE+1],line[MAXLINE+1];
    printf("Send message : ");
    while(fgets(message,MAXLINE,stdin)!=NULL){
      sprintf(line,"%s %s",name,message);
      len=strlen(line);
      if(sendto(send_sock,line,strlen(line),0,(struct sockaddr *)&mcast_group,sizeof(mcast_group))<len)
        error_handling("error: sendto\n");
    }
  }
  return 0;
}

void error_handling(char *message){
  fputs(message,stderr);
  fputs("\n",stderr);
  exit(1);
  
}