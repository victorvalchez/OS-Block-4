//Readers writers problem solution:

/*
-------------------------------------------------------------------------------------
Which alternative to the readers and writers problem is solved by the following code? 
    a) Priority for Readers 
    b) Priority for Writers
-------------------------------------------------------------------------------------

void * Lector(void *p) {
    long i=(long)p;
    while(1) {
        pthread_mutex_lock(&mutex);
        if(escribiendo)
            pthread_cond_wait(&leer,&mutex);
        leyendo++;
        pthread_mutex_unlock(&mutex);

        printf("Lector %ld: Dato = %d\n", i, dato);
        sleep(1); //tardo 1s en leer

        pthread_mutex_lock(&mutex);
        leyendo--;
        if(!leyendo)
            pthread_cond_signal(&escribir);
        pthread_mutex_unlock(&mutex);
    }
}

void * Escritor(void * p)
{
   long i=(long)p;
   while(1)
   {
        pthread_mutex_lock(&mutex);
        while(leyendo||escribiendo)
            pthread_cond_wait(&escribir,&mutex);
        escribiendo++;
        pthread_mutex_unlock(&mutex);
        sleep(1); //tardo 1 sg en escribir
        dato+=2;
        printf(“Escritor %ld: Dato = %d\n", i, dato);
        pthread_mutex_lock(&mutex);
        escribiendo--;
        pthread_cond_broadcast(&escribir);
        pthread_cond_broadcast(&leer);
        pthread_mutex_unlock(&mutex);
   }
}

*/