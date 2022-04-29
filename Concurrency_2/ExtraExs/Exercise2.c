#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_BUFFER         10     /* tamanio del buffer */
#define DATOS_A_PRODUCIR   100    /* datos a producir */

pthread_mutex_t mutex;     /* mutex para controlar el acceso al
                           buffer compartido */
pthread_cond_t no_lleno;   /* controla el llenado del buffer */
pthread_cond_t no_vacio;   /* controla el vaciado del buffer */
int n_elementos;           /* numero de elementos en el buffer */

int buffer[MAX_BUFFER];    /* buffer comun */

void *Productor(void *kk)  {   /* codigo del productor */
   int dato, i ,pos = 0;

    for(i=0; i < DATOS_A_PRODUCIR; i++ )  {
        dato = i;       /* producir dato */
        pthread_mutex_lock(&mutex);        /* acceder al buffer */
        while (n_elementos == MAX_BUFFER)  /* si buffer lleno */
            pthread_cond_wait(&no_lleno, &mutex); /* se bloquea */
        buffer[pos] = i;
        printf("produce %d \n", buffer[pos]);     /* produce dato */
        pos = (pos + 1) % MAX_BUFFER;
        n_elementos ++;
        pthread_cond_signal(&no_vacio);   /* buffer no vacio */
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void *Consumidor(void *kk) {   /* codigo del sonsumidor */
    int dato, i ,pos = 0;

    for(i=0; i < DATOS_A_PRODUCIR; i++ ) {
        pthread_mutex_lock(&mutex);    /* acceder al buffer */
        while (n_elementos == 0)       /* si buffer vacio */
            pthread_cond_wait(&no_vacio, &mutex); /* se bloquea */
        dato = buffer[pos];
        pos = (pos + 1) % MAX_BUFFER;
        n_elementos --;
        pthread_cond_signal(&no_lleno);   /* buffer no lleno */
        pthread_mutex_unlock(&mutex);
        printf("Consume %d \n", dato);     /* consume dato */
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]){
    pthread_t th1, th2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&no_lleno, NULL);
    pthread_cond_init(&no_vacio, NULL);
    pthread_create(&th1, NULL, Productor, NULL);
    pthread_create(&th2, NULL, Consumidor, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&no_lleno);
    pthread_cond_destroy(&no_vacio);

    exit(0);
}
