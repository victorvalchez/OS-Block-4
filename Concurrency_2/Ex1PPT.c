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

int dato_compartido = 0; // para ir poniendo el numero de resultado 0-199
int par = 0;  //0 False, 1 True
pthread_mutex_t m;
pthread_cond_t cL, cV;

//Option A not working

//Option B: working
//COMO SE USA UNA VARIABLE COMPARTIDA, PUES CADA LOOP SE HACE 100 VECES
void pares(void)
{
    int i;
    for(i=0; i < 100; i++ ) //100 de uno
    {
        pthread_mutex_lock(&m);
        if (!par) // == 0
        {
            pthread_cond_wait(&cL,&m);
        }
        printf("Thread1 = %d \n", dato_compartido++);
        par=0;
        pthread_cond_signal(&cV);
        pthread_mutex_unlock(&m);
    }
}

void impares(void)
{
   int i;
   for(i=0; i < 100; i++ ) // y 100 del otro
   {
        pthread_mutex_lock(&m);
        if (par)
        {
            pthread_cond_wait(&cV,&m);
        }
        printf("Thread2 = %d \n", dato_compartido++);
        par=1;
        pthread_cond_signal(&cL);
        pthread_mutex_unlock(&m);
    }
}

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
//Option C: not working, imprime th1 la mitad y th2 la otra mitad
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
*/

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
