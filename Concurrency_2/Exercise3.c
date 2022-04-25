#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_BUFFER          128    /* tamanio del buffer */
#define DATOS_A_PRODUCIR   500    /* datos a producir */

sem_t elementos;                /* elementos en el buffer */
sem_t huecos;                   /* huecos en el buffer */
int buffer[MAX_BUFFER];         /* buffer comun */

void *Consumidor(void *kk);
void *Productor(void *kk);

int main(void)
{
   pthread_t th1, th2;  /* identificadores de threads */

   /* inicializar los semaforos */
   sem_init(&elementos, 0, 0);
   sem_init(&huecos, 0, MAX_BUFFER);
/* crear los procesos ligeros */
   pthread_create(&th1, NULL, Productor, NULL);
   pthread_create(&th2, NULL, Consumidor, NULL);

   /* esperar su finalizacion */
   pthread_join(th1, NULL);
   pthread_join(th2, NULL);

   sem_destroy(&huecos);
   sem_destroy(&elementos);
   exit(0);
}

void *Productor(void *kk)   /* codigo del productor */
{
   int pos = 0;  /* posicion dentro del buffer */
   int dato;     /* dato a producir */
   int i;

   for(i=0; i < DATOS_A_PRODUCIR; i++ )   {
      dato = i;           /* producir dato */
      sem_wait(&huecos);  /* un hueco menos */
      buffer[pos] = dato;
      printf("produce %d \n", buffer[pos]);     /* produce dato */
      pos = (pos + 1) % MAX_BUFFER;
      sem_post(&elementos);  /* un elemento mas */
  }
  pthread_exit(0);
}

void *Consumidor(void *kk)  /* codigo del Consumidor */
{
   int pos = 0;
   int dato;
   int i;

   for(i=0; i < DATOS_A_PRODUCIR; i++ ) {
      sem_wait(&elementos);    /* un elemento menos */
      dato = buffer[pos];
      pos = (pos + 1) % MAX_BUFFER;
      sem_post(&huecos);    /* un hueco mas */
      printf("Consume %d \n", dato);     /* consume dato */
   }
   pthread_exit(0);
}
