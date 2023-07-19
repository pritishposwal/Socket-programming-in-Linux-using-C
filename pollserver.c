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
#define PORT 8080
int newfd;
struct sockaddr_in servaddr;
struct sockaddr_in cliaddr;
int main(){
    FILE *fp=fopen("result5.txt","w");
    int retval;
    socklen_t addr_size=sizeof(cliaddr);
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
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
    int listenfd=listen(sockfd,5);
    if(listenfd<0)
    {
        printf("Listening failed\n");
        return 0;
    }
    struct pollfd fds[11];
    fds[0].fd=sockfd;
    fds[0].events=POLLIN;
    for(int i=1;i<11;i++)
    {
        fds[i].fd=-1;
    }
    int nfds=1;
    printf("The pid of the server is %d \n",getpid());
    while(1)
    {
        retval=poll(fds,nfds,-1);
        if(retval==-1)
        {
            printf("Polling failed\n");
            return 0;
        }
        if(fds[0].revents & POLLIN)
        {
            newfd=accept(sockfd,(struct sockaddr *)&cliaddr,(socklen_t *)&addr_size);
            if(newfd<0)
            {
                printf("Accept failed\n");
                return 0;
            }
            int i;
            for(i=1;i<11;i++)
            {
                if(fds[i].fd==-1)
                {
                    fds[i].fd=newfd;
                    fds[i].events=POLLIN;
                    break;
                }
            }
            nfds++;
        }
        for(int i=1;i<11;i++)
        {
            if(fds[i].fd==-1)
            {
                continue;
            }
            if(fds[i].revents & POLLIN)
            {
                int n;
                int tempread = read(fds[i].fd,&n,sizeof(n));
                if(tempread==0)
                {
                    close(fds[i].fd);
                    fds[i].fd=-1;
                    continue;
                }
                long fact=1;
                for(int j=1;j<=n;j++)
                {
                    fact=fact*j;
                }
                write(fds[i].fd,&fact,sizeof(fact));
                char *temp = inet_ntoa(cliaddr.sin_addr);
                int port = ntohs(cliaddr.sin_port);
                FILE *fp=fopen("result5.txt","a");
                fprintf(fp,"Client IP: %s\tClient Port: %d\tFactorial: %ld",temp,port,fact);
                close(fds[i].fd);
                fds[i].fd=-1;
                nfds--;
                fclose(fp);
            }
        }
    }
    return 0;
}