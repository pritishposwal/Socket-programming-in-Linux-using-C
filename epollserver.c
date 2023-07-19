#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/time.h>
#include<poll.h>
#include<sys/epoll.h>
#define PORT 8080
int newfd;
struct sockaddr_in servaddr;
struct sockaddr_in cliaddr;
int maxvalue;
int main(){
    maxvalue=10000;
    int retval;
    socklen_t addr_size=sizeof(cliaddr);
    struct epoll_event ev;
    struct epoll_event events[maxvalue];
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        printf("Socket creation failed\n");
        return 0;
    }
    servaddr.sin_addr.s_addr=INADDR_ANY;
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
    int bindfd=bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(bindfd<0)
    {
        printf("Binding failed\n");
        return 0;
    }
    int epollfiledescriptor=epoll_create1(0);
    if(epollfiledescriptor<0)
    {
        printf("Epoll creation failed\n");
        return 0;
    }
    ev.data.fd=sockfd;
    ev.events=EPOLLIN;
    int store=epoll_ctl(epollfiledescriptor,EPOLL_CTL_ADD,sockfd,&ev);
    int tempsock=0;
    FILE *fp1=fopen("result6.txt","w");
    for(int i=1;i<maxvalue;i++)
    {
        events[i].data.fd=0;
        events[i].events=POLLIN;
    }
    printf("The pid of the server is %d \n",getpid());
    while(1)
    {
        usleep(1000);
        retval=epoll_wait(epollfiledescriptor,events,maxvalue,-1);
        for(int i=0;i<retval;i++)
        {
            int tempfd=events[i].data.fd;
            for(int j=1;j<21;j++){
                int n=0;
                long fact=1;
                int store=recvfrom(tempfd,&n,sizeof(n),0,(struct sockaddr *)&cliaddr,(socklen_t *)&addr_size);
                for(int k=1;k<=n;k++)
                {
                    fact=fact*k;
                }
                int writestore=sendto(tempfd,(void *)&fact,sizeof(fact),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
                FILE *fp1=fopen("result6.txt","a");
                fprintf(fp1,"Client IP: %s Port Number: %d Factorial of %d is : %ld \n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port),n,fact);
                fclose(fp1);
            }
        }   
    }
    return 0;
}