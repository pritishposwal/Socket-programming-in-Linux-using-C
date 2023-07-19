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
#define PORT 8080
int newfd;
struct sockaddr_in servaddr;
struct sockaddr_in cliaddr;
void *clientconnecter(void *threadid){
    int tid=*(int *)threadid;
    for(int i=1;i<=20;i++)
    {
        int n;
        read(newfd,&n,sizeof(n));
        long fact=1;
        for(int j=1;j<=n;j++)
        {
            fact=fact*j;
        }
        write(newfd,&fact,sizeof(fact));
        FILE *fp=fopen("result3.txt","a");
        char *tempip=inet_ntoa(cliaddr.sin_addr);
        int port=ntohs(cliaddr.sin_port);
        fprintf(fp,"Client IP Address %s and port Address %d Factorial of %d is %ld \n",tempip,port,n,fact);
        fclose(fp);
        usleep(100);
    }
    pthread_exit(NULL);
    return 0;
}
int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    FILE *fp=fopen("result3.txt","w");
    servaddr.sin_addr.s_addr=INADDR_ANY;
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
    socklen_t addr_size;
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
    pthread_t tid[10];
    int i=0;
    printf("The pid of the server is %d \n",getpid());
    while(1)
    {
        int tempstore=sizeof(servaddr);
        newfd=accept(sockfd,(struct sockaddr *)&cliaddr,&tempstore);
        if(newfd<0)
        {
            printf("Accept failed\n");
            return 0;
        }
        if(pthread_create(&tid[i++],NULL,clientconnecter,(void *)&newfd)<0)
        {
            printf("Thread creation failed\n");
            return 0;
        }
        if(i>=10)
        {
            i=0;
            while(i<10)
            {
                pthread_join(tid[i++],NULL);
            }
            i=0;
        }
    }
    close(newfd);
    return 0;
}