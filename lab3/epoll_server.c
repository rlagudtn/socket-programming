#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50
void error_handling(char *message);

int main(int argc,char *argv[]){
  int serv_sock,clnt_sock,epfd;
  struct sockaddr_in serv_adr,clnt_adr;
  socklen_t adr_sz;
  char buf[BUF_SIZE];

  struct epoll_event *ep_events;
  struct epoll_event event;
  int event_cnt;

  if (argc!=2){
    printf("Usage: %s <port>\n",argv[0]);
    exit(1);
  }
  serv_sock=socket(PF_INET,SOCK_STREAM,0);
  memset(&serv_adr,0,sizeof(serv_adr));
  serv_adr.sin_family=AF_INET;
  serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_adr.sin_port=htons(atoi(argv[1]));

  if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
    error_handling("bind() error");
  if(listen(serv_sock,5)==-1)
    error_handling("listen() error");

  //epoll fd 생성
  epfd=epoll_create(EPOLL_SIZE);

  //epoll_events 자료구조 동적할당-> 이벤트 처리된 정보 받아옴
  ep_events=malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

  //소켓 등록을 위한 event 초기화
  event.events=EPOLLIN;
  event.data.fd=serv_sock;
  epoll_ctl(epfd,EPOLL_CTL_ADD,serv_sock,&event);

  while(1){
    event_cnt=epoll_wait(epfd,ep_events,EPOLL_SIZE,-1);
    if(event_cnt==-1){
      printf("epoll_wait error()\n");
      break;
    }
    for (int i=0;i<event_cnt;i++){
      if(ep_events[i].data.fd==serv_sock){ //연결 요청 이벤트가 발생한 경우
        adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,adr_sz);
        event.events=EPOLL_CTL_ADD;
        event.data.fd=clnt_sock;
        epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sock,&event);
        printf("Connected client: %d\n",clnt_sock);
      }
      else{ //그 외 이벤트가 발생한 경우(데이터 송수신)
        str_len=read(ep_events[i].data.fd,buf,BUF_SIZE);
        if(str_len!=0){
          write(ep_events[i].data.fd,buf,str_len);
        }
        else{ //str_len ==0 -> 연결 요청 종료
          epoll_ctl(epfd,EPOLL_CTL_DEL,ep_events[i].data.fd,NULL);
          close(ep_events[i].data.fd);
          printf("closed client: %d\n",ep_events[i].data.fd);
        }
      }
    }

  }
  close(serv_sock);
  close(epfd);

  return 0;

}

void error_handling(char *message){
  fputs(message,stderr);
  fputs("\n",stderr);
  exit(1);
  
}