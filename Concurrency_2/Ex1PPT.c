/*
Given the following code, it is requested to implement the odd and even functions so that one thread prints the even numbers from 0 to 
199 and the other, the odd numbers from 0 to 199 consecutively. An example of the output would be the following:
    Thread2 = 0
    Thread1 = 1
    Thread2 = 2
    Thread1 = 3
    Thread2 = 4
    Thread1 = 5
    Thread2 = 6
    Thread1 = 7
    Thread2 = 8
*/

//Given code
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int dato_compartido = 0;
int es_par = 0;
pthread_mutex_t m;
pthread_cond_t cL, cV;

//Option A not working

//Option B: working
/*
void pares(void)
{
    int i;
    for(i=0; i < 100; i++ )
    {
        pthread_mutex_lock(&m);
        if (es_par==0)
        {
            pthread_cond_wait(&cL,&m);
        }
        printf("Thread1 = %d \n", dato_compartido++);
        es_par=0;
        pthread_cond_signal(&cV);
        pthread_mutex_unlock(&m);
    }
}

void impares(void)
{
   int i;
   for(i=0; i < 100; i++ )
   {
        pthread_mutex_lock(&m);
        if (es_par==1)
        {
            pthread_cond_wait(&cV,&m);
        }
        printf("Thread2 = %d \n", dato_compartido++);
        es_par=1;
        pthread_cond_signal(&cL);
        pthread_mutex_unlock(&m);
    }
}
*/

//Option C: working

void pares(void)
{
    int i;
    for(i=0; i < 100; i++ )
    {
        pthread_mutex_lock(&m);
        if (es_par==0)
        {
            pthread_cond_wait(&cL,&m);
        }
        printf("Thread1 = %d \n", dato_compartido++);
        es_par=1;
        pthread_cond_signal(&cV);
        pthread_mutex_unlock(&m);
    }
}

void impares(void)
{
   int i;
   for(i=0; i < 100; i++ )
   {
        pthread_mutex_lock(&m);
        if (es_par==1)
        {
            pthread_cond_wait(&cV,&m);
        }
        printf("Thread2 = %d \n", dato_compartido++);
        es_par=0;
        pthread_cond_signal(&cL);
        pthread_mutex_unlock(&m);
    }
}


//Option D: not working, imprime 399 veces
/*
void pares(void)
{
    int i;
    for(i=0; i < 200; i++ )
    {
        pthread_mutex_lock(&m);
        if (es_par==0)
        {
            pthread_cond_wait(&cL,&m);
        }
        printf("Thread1 = %d \n", dato_compartido++);
        es_par=0;
        pthread_cond_signal(&cV);
        pthread_mutex_unlock(&m);
    }
}

void impares(void)
{
   int i;
   for(i=0; i < 200; i++ )
   {
        pthread_mutex_lock(&m);
        if (es_par==1)
        {
            pthread_cond_wait(&cV,&m);
        }
        printf("Thread2 = %d \n", dato_compartido++);
        es_par=1;
        pthread_cond_signal(&cL);
        pthread_mutex_unlock(&m);
    }
}
*/

int main(void) {
    pthread_t th1, th2;
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&cL, NULL);
    pthread_cond_init(&cV, NULL);
    pthread_create(&th1, NULL, (void *) pares, NULL);
    pthread_create(&th2, NULL, (void *) impares, NULL);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&cL);
    pthread_cond_destroy(&cV);
    return 0;
}