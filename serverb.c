#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#define PORT 8080
int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    servaddr.sin_addr.s_addr=INADDR_ANY;
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
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
    printf("The pid of the server is %d \n",getpid());
    FILE *fp=fopen("result2.txt","w");
    while(1)
    {
        int tempstore=sizeof(servaddr);
        int newsockfd=accept(sockfd,(struct sockaddr *)&servaddr,&tempstore);
        if(newsockfd<0)
        {
            printf("Accept failed\n");
            return 0;
        }
        int pid=fork();
        if(pid==0)
        {
            close(sockfd);
            for(int j=0;j<20;j++)
            {
                int n;
                read(newsockfd,&n,sizeof(n));
                long fact=1;
                for(int i=1;i<=n;i++)
                    fact*=i;
                write(newsockfd,&fact,sizeof(fact));
                FILE *fp=fopen("result2.txt","a");
                char *tempip=inet_ntoa(cliaddr.sin_addr);
                int port=ntohs(cliaddr.sin_port);
                fprintf(fp,"Client IP Address %s and port Address %d Factorial of %d is %ld \n",tempip,port,n,fact);
                fclose(fp);
                usleep(1000);
            }
            exit(0);
        }
        else
        {
            close(newsockfd);
        }
    }
    return 0;
}