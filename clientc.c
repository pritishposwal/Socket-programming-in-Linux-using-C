#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<math.h>
#define PORT 8080
double time_spent=0;
void *clientconnecter(void *threadid){
    int tid=*(int *)threadid;
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
    for(int i=1;i<=20;i++)
    {
        write(sockfd,&i,sizeof(i));
        clock_t begin=clock();
        long fact;
        read(sockfd,&fact,sizeof(fact));
        clock_t finish=clock();
        double temp=fabs(begin-finish);
        double temp2=temp/CLOCKS_PER_SEC;
        time_spent+=(double)temp2;
        printf("For client %d Factorial of %d is %ld \n",tid,i,fact);
        usleep(100);
    }
    close(sockfd);
    return 0;
}
int main()
{
    pthread_t tempthread[10];
    for(int i=0;i<10;i++)
    {
        int temp=pthread_create(&tempthread[i],NULL,clientconnecter,&i);
        if(temp<0)
        {
            printf("Thread creation failed\n");
            return 0;
        }
        usleep(10000);
    }
    for(int i=0;i<10;i++)
    {
        pthread_join(tempthread[i],NULL);
    }
    printf("Time taken for 10 clients is %f seconds", time_spent);
    return 0;
}

