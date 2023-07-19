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
#define PORT 8080
int newfd;
struct sockaddr_in servaddr;
struct sockaddr_in cliaddr;
int main(){
    FILE *fp=fopen("result4.txt","w");
    int clientsocket[10];
    servaddr.sin_addr.s_addr=INADDR_ANY;
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
    socklen_t addr_size;
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        printf("Socket creation failed\n");
        return 0;
    }
    int bindfd=bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(bindfd<0)
    {
        printf("Binding failed\n");
        return 0;
    }
    int listenfd=listen(sockfd,5);
    if(listenfd<0)
    {
        printf("Listening failed\n");
        return 0;
    }
    for(int i=0;i<10;i++)
    {
        clientsocket[i]=0;
    }
    int addrsize=sizeof(cliaddr);
    fd_set readfds;
    int activity,maxfd;
    printf("The pid of the server is %d \n",getpid());
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(sockfd,&readfds);
        maxfd=sockfd;
        for(int i=0;i<10;i++)
        {
            if(clientsocket[i]>0)
            {
                FD_SET(clientsocket[i],&readfds);
            }
            if(clientsocket[i]>maxfd)
            {
                maxfd=clientsocket[i];
            }
        }
        activity=select(maxfd+1,&readfds,NULL,NULL,NULL);
        if(activity<0)
        {
            printf("Error in select\n");
        }
        if(FD_ISSET(sockfd,&readfds))
        {
            newfd=accept(sockfd,(struct sockaddr *)&cliaddr,(socklen_t *)&addrsize);
            if(newfd<0)
            {
                printf("Accept failed\n");
                return 0;
            }
            for(int i=0;i<10;i++)
            {
                if(clientsocket[i]==0)
                {
                    clientsocket[i]=newfd;
                    break;
                }
            }
        }
        for(int i=0;i<10;i++)
        {
            if(FD_ISSET(clientsocket[i],&readfds))
            {
                int n;
                int tempread=read(clientsocket[i],&n,sizeof(n));
                if(tempread==0)
                {
                    close(clientsocket[i]);
                    clientsocket[i]=0;
                }
                else
                {
                    long fact=1;
                    for(int j=1;j<=n;j++)
                    {
                        fact=fact*j;
                    }
                    write(clientsocket[i],&fact,sizeof(fact));
                    char *tempip=inet_ntoa(cliaddr.sin_addr);
                    int port=ntohs(cliaddr.sin_port);
                    FILE *fp=fopen("result4.txt","a");
                    fprintf(fp,"Client IP Address %s and port Address %d Factorial of %d is %ld \n",tempip,port,n,fact);
                    fclose(fp);
                }
            }
        }
    }
    return 0;
}