/*
|-----------------|
|READERS - WRITERS|
|-----------------|
*/
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>

sem_t rd;
sem_t wr;
int nreader = 0;
int data = 5;

void reader(){
    sem_wait(&rd);
    nreader++;
    if (nreader == 1){
        sem_wait(&wr);
    }
    sem_post(&rd);

    //Perform read
    printf("Number is: %d\n", data);

    sem_wait(&rd);
    nreader--;
    if (nreader == 0){
        sem_post(&wr);
    }
    sem_post(&rd);
    pthread_exit(0);
}

void writer(){
    sem_wait(&wr);
    data += 2;
    sem_post(&wr);

    pthread_exit(0);
}

int main(){
    pthread_t th1, th2, th3, th4;
    
    sem_init(&rd, 0, 1);
    sem_init(&wr, 0, 0);

    pthread_create(&th1, NULL, reader, NULL);
    pthread_create(&th2, NULL, writer, NULL);
    pthread_create(&th3, NULL, reader, NULL);
    pthread_create(&th4, NULL, writer, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    pthread_join(th4, NULL);

    sem_destroy(&rd);
    sem_destroy(&wr);
}