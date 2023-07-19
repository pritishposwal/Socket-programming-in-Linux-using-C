#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 8080
int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        printf("Socket creation failed");
        return 0;
    }
    //server side process
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
    servaddr.sin_addr.s_addr=INADDR_ANY;
    int bindfd=bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(bindfd<0){
        printf("Binding failed \n");
        return 0;
    }
    int listenfd=listen(sockfd,5);
    if(listenfd<0){
        printf("Listening failed \n");
        return 0;
    }
    int tempstore=sizeof(servaddr);
    int newsockfd=accept(sockfd,(struct sockaddr *)&cliaddr,&tempstore);
    if(newsockfd<0){
            printf("Accept failed");
            return 0;
        }
    FILE *fp=fopen("result.txt","w");
    while(1){
        usleep(100);
        int n;
        read(newsockfd,&n,sizeof(n));
        long fact=1;
        for(int i=1;i<=n;i++)
            fact*=i;
        write(newsockfd,&fact,sizeof(fact));
        FILE *fp=fopen("result.txt","a");
        char *tempip=inet_ntoa(cliaddr.sin_addr);
        int port=ntohs(cliaddr.sin_port);
        fprintf(fp,"Client IP Address %s and port Address %d Factorial of %d is %ld \n",tempip,port,n,fact);
        fclose(fp);
    }
    close(sockfd);
    return 0;
}