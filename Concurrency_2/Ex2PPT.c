/*
A student has provided us with the following code, in which different threads are created from the main process 
that execute the print () function

VER COMO FUNCIONA Y ENTENDER
*/

//Todo el code dado en el enunciado
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define N 3

pthread_t thread1, thread2; 
pthread_attr_t attr; /*atributos de los threads*/
pthread_mutex_t impresor = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t imprimirHola;
pthread_cond_t imprimirMundo;
int impHola = 1;

void *imprimir (void *arg)
{
        char cadena_hola[] = "Hola ";
        char a[12];
        pthread_mutex_lock (&impresor);
        strcpy(a, (char*)arg);
        if (strncmp(a,cadena_hola,5)==0) {
                while (impHola == 0) {
                        pthread_cond_wait(&imprimirHola,&impresor);
                }
                printf("%s", a);
                impHola = 0;
                pthread_cond_signal(&imprimirMundo);
        } else {
                while (impHola == 1){
                        pthread_cond_wait(&imprimirMundo,&impresor);
                }
                printf("%s", a);
                impHola = 1;
                pthread_cond_signal(&imprimirHola);
        }
        pthread_mutex_unlock (&impresor);
        pthread_exit (NULL);
}

int main (void)
{
    pthread_cond_init(&imprimirHola, NULL);
    pthread_cond_init(&imprimirMundo, NULL);
    char cadena_hola[]="Hola ";
    char cadena_mundo[]="mundo \n";
    int i;
    pthread_attr_init (&attr);
    for (i=1; i<=N; i++) {
        pthread_create(&thread2, &attr, imprimir, (void *)cadena_mundo);
        pthread_create(&thread1, &attr, imprimir, (void *)cadena_hola);
    }
    pthread_exit (NULL);
}
