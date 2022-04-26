/* COMPILAR CON:
------------------
gcc -Wall -g -o pc prod_cons.c -lpthread
------------------
*/

#include <stdio.h>
#include <pthread.h>

#define MAX_BUFFER 5
#define N_PRODUCTORES 1
#define N_COSUMIDORES 1
#define MAX_ELEMS 10

int buffer[MAX_BUFFER];
int n_elementos = 0;
int fin = 0;

int ha_arrancado = 0; //0: false, 1: true
pthread_mutex_t mutex;
pthread_cond_t arrancado;
pthread_cond_t no_vacio;
pthread_cond_t no_lleno;

void * productor(void * param){
    int id;
    int p; //numeor entero a producir
    int pos = 0;

    // hemos arrancado el hilo prodcutor
    pthread_mutex_lock(&mutex);
    ha_arrancado = 1;
    id = *((int *)param);
    pthread_cond_signal(&arrancado);
    pthread_mutex_unlock(&mutex);

    // producir
    for (int i=0; i<MAX_ELEMS; i++){
        p = i;
        printf("Productor = %d; PETI_ID = %d; PETI_VALOR = %d\n", id, i, p);
        
        // entrar en critical sect si no esta lleno
        pthread_mutex_lock(&mutex);
        while (n_elementos == MAX_BUFFER){ //MIENTRAS ESTE LLENO EL BUFFER
            pthread_cond_wait(&no_lleno, &mutex);
        }

        //insertar en el buffercircular el elemneto porducido
        buffer[pos] = p;
        pos = (pos + 1) % MAX_BUFFER; /* Esto sirve para cuando queremos que el buffer sea circular es decir si tenemos un buffer 
        con 5 huecos, que al llegar al final vuelva a modifiucar el hueco inicial, y asi hasta acabar con todos los productores */
        n_elementos++;

        //sakir de la crit section y avisar que no etsa vacio y que hay algun elemnto que pueda ulitlizar
        pthread_cond_signal(&no_vacio);  //Cuando metamos algun elemenot avisamos al consumidor de que ya no esta vacio con rsta signal
        pthread_mutex_unlock(&mutex);
    }

    //fin ejecucion del productor
    printf("Productor = %d; FIN\n", id);
    pthread_exit(0);
    return NULL;  //Esta linea no se debe ejecutar ya que hacemos el exit
}

void * consumidor(void * param){
    int id;
    int p;
    int pos = 0;

    // hemos arrancado el hilo consumidor
    pthread_mutex_lock(&mutex);
    ha_arrancado = 1;
    id = *((int *)param);
    pthread_cond_signal(&arrancado);
    pthread_mutex_unlock(&mutex);

    // consumir
    for (int i=0; ; i++){
        // entrar en crit sect, si no esta vacio
        pthread_mutex_lock(&mutex);
        while (n_elementos == 0) { //esta vaciom es decir que hemos entrado antes que el productor, hay que dormir al thread
            // esta vacio y hemos acabado ya
            if (fin == 1){
                printf("Consumidor = %d; FIN\n", id);
                pthread_mutex_unlock(&mutex);
                pthread_exit((void*)1);
            }

            //Esta vacio pero no hemos terminado
            pthread_cond_wait(&no_vacio, &mutex); //la condicion de que la otra parte avise de que no esta vacio y asi puedes accerder
        }
        
        //quiatr elto del biffer ciruclar y consumirlo
        p = buffer[pos];
        pos = (pos + 1) % MAX_BUFFER; /* Esto sirve para cuando queremos que el buffer sea circular es decir si tenemos un buffer 
        con 5 huecos, que al llegar al final vuelva a modifiucar el hueco inicial, y asi hasta acabar con todos los productores */
        n_elementos--;

        //salir de la sc y avisar de que ya hay un hueco y no esta lleno
        pthread_cond_signal(&no_lleno);
        pthread_mutex_unlock(&mutex);

        printf("Consumidor = %d; PETI_ID = %d; PETI_VALOR = %d\n", id, i, p);
    }

    //fin ejecucion del consumidor
    printf("Consumidor = %d; FIN\n", id);
    pthread_exit(0);
    return NULL;  //Esta linea no se debe ejecutar ya que hacemos el exit
}

int main(int argc, char *argv[]){
	void *retval;
	
    pthread_t threads[N_PRODUCTORES + N_COSUMIDORES]; //THREAD POOL
    //int i = 0; (se puede hacer dentro del loop)
    
    //INICIALIZAR
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&arrancado, NULL);
    pthread_cond_init(&no_lleno, NULL);
    pthread_cond_init(&no_vacio, NULL);

    // crear PRODUCTORES
    for (int i=0; i < N_PRODUCTORES; i++){
        pthread_create(&(threads[i]), NULL, productor, &i); //Para cada hilo, creamos un productorm y 
        //le damos valores por defector con los parametros de i, que tenemos que pasarle la direccion de i (con &)
        pthread_mutex_lock(&mutex);
        while(ha_arrancado != 0){
            pthread_cond_wait(&arrancado, &mutex); /*Te qurdas dormido en ha arrancado, y metex el mutex para que se haga el unlock del mutex
            y se quede el proceso ahi dormido, y la otra parte hara el signal para despertar el proceso y el while reevalua si ha_arrancado */
        }
        ha_arrancado = 0;
        pthread_mutex_unlock(&mutex);
    }

    // crear CONSUMIDORES
    for (int i=0; i < N_COSUMIDORES; i++){
        pthread_create(&(threads[N_PRODUCTORES + i]), NULL, consumidor, &i); //Para cada hilo (sera el n_productor + i ya que el n de hilos es la suma de ambos), 
        //creamos un consumidor y le damos valores por defector con ningun parametro(null) 
        pthread_mutex_lock(&mutex);
        while(!ha_arrancado){
            pthread_cond_wait(&arrancado, &mutex); /*Te qurdas dormido en ha arrancado, y metex el mutex para que se haga el unlock del mutex
            y se quede el proceso ahi dormido, y la otra parte hara el signal para despertar el proceso y el while reevalua si ha_arrancado */
        }
        ha_arrancado = 0;
        pthread_mutex_unlock(&mutex);
    }

    // esperar que terminen los PRODUCTORES
    for (int i=0; i < N_PRODUCTORES; i++){
        pthread_join(threads[i], &retval);  //Donde va null es la direccion donde queremos que deje el estado de ejecucion del hilo,
        // al poner null se descarta
    }

    // fin ejecucion CONSUMIDORES
    pthread_mutex_lock(&mutex);
    fin = 1;
    pthread_cond_broadcast(&no_vacio);
    pthread_mutex_unlock(&mutex);

    // esperar que terminen los CONSUMIDORES
    for (int i=0; i < N_COSUMIDORES; i++){
        pthread_join(threads[i + N_PRODUCTORES], &retval);  
    }

    //FINALIZAR VARIABLES
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&arrancado);
    pthread_cond_destroy(&no_lleno);
    pthread_cond_destroy(&no_vacio);
    return 0;
}
