#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<math.h>
#define PORT 8080
int i=1;
double timespent=0;
int threadsreq=20*10;
void *clientconnecter(void *threadid){
    int tid=*(int *)threadid;
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in cliaddr;
    cliaddr.sin_addr.s_addr=INADDR_ANY;
    cliaddr.sin_family=AF_INET;
    cliaddr.sin_port=htons(PORT);
    if(sockfd<0)
    {
        printf("Socket creation failed\n");
        return 0;
    }
    int clientfd=connect(sockfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
    if(clientfd<0)
    {
        printf("Connection failed\n");
        return 0;
    }
    write(sockfd,&i,sizeof(i));
    clock_t begin=clock();
    long fact;
    read(sockfd,&fact,sizeof(fact));
    clock_t finish=clock();
    double temp=fabs(begin-finish);
    double temp2=temp/CLOCKS_PER_SEC;
    timespent+=(double)temp2;
    printf("For Client %d Factorial of %d is %ld \n",tid,i,fact);
    i++;
    if(i>20)
    {
        i-=20;
    }
    usleep(1000);
    close(sockfd);
    return 0;
}
int main()
{
    pthread_t tempthread[threadsreq];
    for(int i=0;i<threadsreq;i++)
    {
        int temp=pthread_create(&tempthread[i],NULL,clientconnecter,&i);
        if(temp<0)
        {
            printf("Thread creation failed\n");
            return 0;
        }
        usleep(1000);
    }
    for(int i=0;i<threadsreq;i++)
    {
        pthread_join(tempthread[i],NULL);
    }
    printf("Time taken for 20 clients is %f seconds\n",timespent);
    return 0;
}

