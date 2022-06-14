#include<bits/stdc++.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
using namespace std;
int cus=0;
int so=0;
int x=0;
sem_t enter,sofa,chair,barber,leave,payment;
pthread_mutex_t lock1,lock2;
queue<int> pay;
queue<int> lev;
void initial(){
    sem_init(&enter,0,20);
    sem_init(&sofa,0,4);
    sem_init(&chair,0,3);
    sem_init(&chair,0,3);
    sem_init(&barber,0,0);
    sem_init(&leave,0,0);
    sem_init(&payment,0,0);

    pthread_mutex_init(&lock1,0);
    pthread_mutex_init(&lock2,0);
}
void *customer(void *arg){
    int id=*(int*)arg;
    sem_wait(&enter);
    printf("Customer%d enters into shop\n",id);
    sleep(1);
    so++;
    if(so>4){
        printf("Customer%d waits for sofa\n",id);
    }
    sem_wait(&sofa);
    printf("Customer%d sits on sofa\n",id);
    pthread_mutex_lock(&lock1);
    sem_wait(&chair);
    sleep(1);
    printf("Customer%d sits on barber chair\n",id);
    sem_post(&sofa);
    x=id;
    sem_post(&barber);
    sem_wait(&leave);
    int b=lev.front();
    printf("Customer%d leaves\n",b);
    lev.pop();

    sem_post(&enter);


}
void *Barber(void *arg){
    int id=*(int*)arg;
    while(1){
        sem_wait(&barber);
        printf("Barber%d cuts hair of customer%d\n",id,x);
        pay.push(x);
        pthread_mutex_unlock(&lock1);
        pthread_mutex_lock(&lock2);
        sem_post(&payment);


    }
}
void *Payment(void *arg){
    while(1){
        sem_wait(&payment);
        int c=pay.front();
        lev.push(pay.front());
        pay.pop();
        printf("Payment Received for customer%d\n",c);
        pthread_mutex_unlock(&lock2);
        sem_post(&leave);
        sem_post(&chair);

    }
}
int main()
{
    initial();
    pthread_t cus[25],bar[3],p;
    int i;


    for(i=0; i<25; i++){
        int* id = (int*)malloc(sizeof(int));
        *id = i+1;
        pthread_create(&cus[i],NULL,customer,id);
    }
    for(i=0; i<3; i++){
        int* id = (int*)malloc(sizeof(int));
        *id = i+1;
        pthread_create(&bar[i],NULL,Barber,id);
    }
    pthread_create(&p,NULL,Payment,NULL);
while(1);
}
