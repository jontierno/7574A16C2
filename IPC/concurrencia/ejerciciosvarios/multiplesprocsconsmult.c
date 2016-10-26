#include "include/measure.h"
#include "include/logger.h"
#include "include/semaforo.h"
#include "include/shm.h"
#include <time.h>
#include <stdlib.h>


/**
    Este ejercicio modela el problema clásico de N procesos con M consumidores con un buffer circular de tamaño limitado
*/


// Cantidad de consumidores
#define N_CONSUMIDORES 2
// Cantidad de productores
#define N_PRODUCTORES 3
//Tamaño del buffer circular
#define LONG_BUFFER 10
//IDs  de semaforos usados-
#define ID_SEM1 1
#define ID_SEM2 2
#define ID_SEM3 3
//IDs de memorias compartidas usadas.
#define ID_SHM1 1
#define ID_SHM2 2

//Los cursores de lectura y escritura se guardan en un array de 
//tres posiciones junto con la cantidad actual de elementos en el buffer
#define LEC_CURSOR 0
#define ESC_CURSOR 1
#define BUFFER_LONG_ACT 2

//Cuantos numeros va a generar cada productor.
#define CANT_NUMEROS_PRODUCTOR 1000
int main(){
    srand (getpid());
    init_logger("Proceso Starter", 0);
    log_exit();
    
    //Semaforo lectura
    int sem_lec = creasem(ID_SEM1);
    inisem(sem_lec,0); 

    //Semaforo escritura
    int sem_esc = creasem(ID_SEM2);
    inisem(sem_esc,1);

    //Semaforo memoria compartida
    int sem_shm = creasem(ID_SEM3);
    inisem(sem_shm,1);

    //CREO LA MEMORIA COMPARTIDA DE LONG_BUFFER NUMEROS
    int memoryhndlr = creashm(ID_SHM1, sizeof(int)*LONG_BUFFER);
    int* buffer = (int*)map(memoryhndlr);

    //inicializo la memoria compartida
    for(int i =0 ; i < LONG_BUFFER; i++){
        buffer[i] = 0;
    }

    //Creo la memoria compartida con dos números para los cursores de lectura y escritura
    int memoryhndlr2 = creashm(ID_SHM2, sizeof(int)*3);
    int* cursores = (int*)map(memoryhndlr2);
    cursores[LEC_CURSOR] = 0;
    cursores[ESC_CURSOR] = 0;
    cursores[BUFFER_LONG_ACT] = 0;

    
    //ejecuto los consumidores
    int consumidor = 0;
    int productor = 0; 
    int numeroproceso = 0;
    for(int i =1; i <= N_CONSUMIDORES && numeroproceso == 0; i++) {
        int pid = fork();
        numeroproceso = pid > 0 ? 0 : i;
        if (numeroproceso > 0) {
            consumidor = 1;
            srand (getpid());
            init_logger("Consumidor", numeroproceso);
            log_exit();
            safelog("Inicia el consumidor %d\n",numeroproceso);    
        }        
    }

    //ejecuto los productores.
    for(int i =1; i <= N_PRODUCTORES && numeroproceso == 0; i++) {
        int pid = fork();
        numeroproceso = pid > 0 ? 0 : i;
        if (numeroproceso > 0) {
            productor = 1;
            srand(getpid());
            init_logger("Productor", numeroproceso + N_CONSUMIDORES);
            log_exit();
            safelog("Inicia el productor %d\n",numeroproceso);    
        }        
    }


    // mapeo la memoria compartida en los procesos creados.
    if (productor || consumidor) {
        buffer = (int*) map( memoryhndlr);
        cursores = (int*) map(memoryhndlr2);
    }
    

    //PROCESO PRODUCTOR
    for (int j=0; j<CANT_NUMEROS_PRODUCTOR && productor;j++){
        usleep((rand() % 5) *1000);
        int gen = rand() % 100;
        //pido permiso de escritura y la shm indicies
        p(sem_esc);  
        p(sem_shm);
        //si hay espacio guardo el numero.
        if(cursores[BUFFER_LONG_ACT] < LONG_BUFFER) {
            safelog("El productor %d genera el numero %d en la posicion %d\n",numeroproceso,gen, cursores[ESC_CURSOR]);    
            buffer[cursores[ESC_CURSOR]] = gen;
            //aumento en uno la cantidad de elementos disponibles.
            cursores[BUFFER_LONG_ACT]++;

            //aumento circularmente el indice.
            cursores[ESC_CURSOR]  = (cursores[ESC_CURSOR] +1)% LONG_BUFFER;
            
            //ahora hay algo para leer
            safelog("El productor %d abre la lectura\n",numeroproceso);    
            v(sem_lec);
            v(sem_esc);
        }
        v(sem_shm);
    }


    int leido = 0;

    // PROCESO CONSUMIDOR
    while (consumidor) {
        
        usleep((rand() % 5) *1000);

        //pido permiso de lectura y la shm indices.
        p(sem_lec);  
        p(sem_shm);

        if(cursores[BUFFER_LONG_ACT] > 0 ) {
            
            leido = buffer[cursores[LEC_CURSOR]];

            // ahora hay un elemento menos disponible.
            cursores[BUFFER_LONG_ACT] --;
            
            //aumento el cursor de lectura.
            
            safelog("El lector %d lee el numero %d de la posicion %d\n",numeroproceso,leido,cursores[LEC_CURSOR]);    
            cursores[LEC_CURSOR] = (cursores[LEC_CURSOR]+1)  % LONG_BUFFER;
            //despues de esto se puede seguir leyendo y escribiendo.
            v(sem_lec);
            v(sem_esc);
        }
        v(sem_shm);
    }

    if (consumidor) safelog("Finalizando Consumidor %d \n", numeroproceso);
    if (productor) safelog("Finalizando Productor %d \n", numeroproceso);

}
