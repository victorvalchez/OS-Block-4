/*
Implement a program that solves the producer-consumer problem with MUTEX. The program describes two threads, producer and consumer, 
that share a finite size buffer. 
    - The producer's task is to generate an integer, store it, and start over; 
    - The consumer takes (simultaneously) numbers one by one. 
    - The problem is that the producer does not add more numbers than the buffer capacity and that the consumer does not try to take a 
    number if the buffer is empty.
*/

// ---- PSEUDOCODE ----
/*
definir buffer
definir datos a producir

proceso principal:
	crear un hilo para el productor	
	crear un hilo para el consumidor
	esperar a que acaben los dos hilos	
	finalizar

proceso productor:
	definir el primer hueco donde colocar el dato
	bucle desde 0 hasta el número de datos a producir
		almacenar en la posición correspondiente el datoproducido
		escribir en pantalla qué datos hemos producido
		calcular la  nueva posición del buffer donde almacenar el datosiguiente
	terminar

proceso consumidor:
	definir el primer dato que vamos a consumir
	bucle desde 0 hasta el número de datos a producir
		consumir el dato
		calcular la  nueva posición del buffer donde recoger el dato siguiente
                     escribir en pantalla qué datos hemos consumido
	terminar

*/
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define DATOS_A_PRODUCIR 5
#define MAX_BUFFER 1024
char buffer[MAX_BUFFER];
void *Productor(void *kk)   /* codigo del productor */
{
    int pos = 0;  /* posicion dentro del buffer */
    int dato;     /* dato a producir */
    int i;
    sem_t holes;
    sem_t elements;

    for(i=0; i < DATOS_A_PRODUCIR; i++ )   {
      dato = i;           /* producir dato */
      sem_wait(&holes);  /* un hueco menos */
      buffer[pos] = dato;
      printf("produce %d \n", buffer[pos]);     /* produce dato */
      pos = (pos + 1) % MAX_BUFFER;
      sem_post(&elements);  /* un elemento mas */
  }
  pthread_exit(0);
}

void *Consumidor(void *kk)  /* codigo del Consumidor */
{
   int pos = 0;
   int dato;
   int i;
   sem_t holes;
   sem_t elements;

   for(i=0; i < DATOS_A_PRODUCIR; i++ ) {
      sem_wait(&elements);    /* un elemento menos */
      dato = buffer[pos];
      pos = (pos + 1) % MAX_BUFFER;
      sem_post(&holes);    /* un hueco mas */
      printf("Consume %d \n", dato);     /* consume dato */
   }
   pthread_exit(0);
}

int main(void)
{
   pthread_t th1, th2;  /* identificadores de threads */
    sem_t holes;
    sem_t elements;

   /* inicializar los semaforos */
   sem_init(&elements, 0, 0);
   sem_init(&holes, 0, MAX_BUFFER);
/* crear los procesos ligeros */
   pthread_create(&th1, NULL, Productor, NULL);
   pthread_create(&th2, NULL, Consumidor, NULL);

   /* esperar su finalizacion */
   pthread_join(th1, NULL);
   pthread_join(th2, NULL);

   sem_destroy(&holes);
   sem_destroy(&elements);
   exit(0);
}
