#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10
#define TAMANIO 1024

void *trabajador(void *arg);

int vector[TAMANIO];

struct b_s {
   int n;
   pthread_mutex_t m;
   pthread_cond_t ll;
}  b;


int main(void) {
   pthread_t hilo[N];
   int i;

   b.n = 0;
   pthread_mutex_init(&b.m, NULL);
   pthread_cond_init(&b.ll, NULL);
  
   for(i=0; i<N; i++)
      pthread_create(&hilo[i],NULL, trabajador, (void *)&i);
  
   for(i=0; i<N; i++)
      pthread_join(hilo[i], NULL);

   pthread_cond_destroy(&b.ll);
   pthread_mutex_destroy(&b.m);

   for(i=0; i<TAMANIO; i++)
      printf("vector [%d] = %d \n", i, vector[i]);

   return 0;
}
void *trabajador(void *arg) {
   int inicio=0, fin=0, i, id;

   id  = *(int *)arg; 
   inicio =(id)*(TAMANIO/N);
   fin = (id+1)*(TAMANIO/N);

   printf ("argumento %d", id);
   for(i=inicio; i<fin; i++) 
      vector[i] = id;
   
   pthread_mutex_lock(&b.m); 
   b.n++; 
   if (N<=b.n) {
      printf("Hilo %ld inicio %d, fin %d, despierto a todos\n", pthread_self(), inicio, fin);
      pthread_cond_broadcast(&b.ll);
   }
   else {
      printf("Hilo %ld inicio %d, fin %d, me duermo\n", pthread_self(), inicio, fin);
      pthread_cond_wait(&b.ll, &b.m); 
   }
   pthread_mutex_unlock(&b.m); 
	
   return 0;
}
