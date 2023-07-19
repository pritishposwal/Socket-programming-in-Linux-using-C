#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 8080
int main()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in servaddr;
    servaddr.sin_addr.s_addr=INADDR_ANY;
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
    if(sockfd<0)
    {
        printf("Socket creation failed\n");
        return 0;
    }
    int clientfd=connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(clientfd<0)
    {
        printf("Connection failed\n");
        return 0;
    }
    else{
        printf("Connection accepted\n");
    }
    for(int i=1;i<=20;i++)
    {
        write(sockfd,&i,sizeof(i));
        long fact;
        read(sockfd,&fact,sizeof(fact));
        printf("Factorial of %d is %ld \n",i,fact);
        usleep(1000);
    }
    close(sockfd);
    return 0;
}
