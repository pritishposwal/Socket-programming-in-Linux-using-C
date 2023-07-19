#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<math.h>
#define PORT 8080
struct sockaddr_in cliaddr;
float timespent=0;
void *clientconnecter(void *threadid){
    int tid=*(int *)threadid;
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        printf("Socket creation failed\n");
        return 0;
    }
    cliaddr.sin_addr.s_addr=INADDR_ANY;
    cliaddr.sin_family=AF_INET;
    cliaddr.sin_port=htons(PORT);
    int tempsize=sizeof(cliaddr);
    for(int i=1;i<=20;i++)
    {
        sendto(sockfd,(void *)&i,sizeof(i),0,(struct sockaddr *)&cliaddr,tempsize);
        long fact=0;
        clock_t begin=clock();
        int recresponse=recvfrom(sockfd,&fact,sizeof(fact),0,NULL,NULL);
        clock_t finish=clock();
        double temp=fabs(begin-finish);
        double temp2=temp/CLOCKS_PER_SEC;
        timespent+=(double)temp2;
        printf("For client %d Factorial of %d is %ld\n",tid,i,fact);
    }
    close(sockfd);
}
int main()
{
    pthread_t tempthread[10];
    for(int i=0;i<10;i++)
    {
        int temp=pthread_create(&tempthread[i],NULL,clientconnecter,(void*)&i);
        if(temp<0)
        {
            printf("Thread creation failed\n");
            return 0;
        }
        usleep(1000);
    }
    for(int i=0;i<10;i++)
    {
        pthread_join(tempthread[i],NULL);
    }
    printf("Time taken for 20 clients is %f seconds\n",timespent);
    return 0;
}

