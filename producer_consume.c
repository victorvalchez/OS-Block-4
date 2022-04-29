//Producer consumer con mutex pero como el ejemplo de los apuntes
#include <stdio.h>
#include <pthread.h>

#define MAX_BUFFER 1024
#define DATOS_A_PRODUCIR 10

pthread_cond_t no_lleno;
pthread_cond_t no_vacio;
pthread_mutex_t mutex;

int n_elements = 0;
int buffer[MAX_BUFFER];

void productor(){
    int data;
    int pos = 0;

    for (int i = 0; i < DATOS_A_PRODUCIR; i++){
        data = i;
        pthread_mutex_lock(&mutex);

        while (n_elements == MAX_BUFFER){
            pthread_cond_wait(&no_lleno, &mutex);
        }

        buffer[pos] = data;  //Guardanos la data en el buffer, en la posicion que toque
        pos = (pos + 1) % MAX_BUFFER;  //Actualizamos la posicion
        n_elements++;  //Y sumamos 1 al numero de elementos que hay en el buffer

        pthread_cond_signal(&no_vacio);
        pthread_mutex_unlock(&mutex);
        printf("Producida %d\n", data);
    }
    printf("Fin Productor\n");
    pthread_exit(0);
}

void consumidor(){
    int data;
    int pos = 0;

    for (int i = 0; i < DATOS_A_PRODUCIR; i++){
        data = i;
        pthread_mutex_lock(&mutex);
        
        while (n_elements == 0){
            pthread_cond_wait(&no_vacio, &mutex);
        }

        data = buffer[pos];
        pos = (pos + 1) % MAX_BUFFER;
        n_elements--;

        pthread_cond_signal(&no_lleno);
        pthread_mutex_unlock(&mutex);
        printf("Consumida %d\n", data);
    }
    printf("Fin Consumidor\n");
    pthread_exit(0);
}
int main(){
    pthread_t th1, th2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&no_lleno, NULL);
    pthread_cond_init(&no_vacio, NULL);

    pthread_create(&th1, NULL, (void *) productor, NULL);
    pthread_create(&th2, NULL, (void *) consumidor, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&no_lleno);
    pthread_cond_destroy(&no_vacio);
    
    return 0;
}
