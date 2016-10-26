#include "include/measure.h"
#include "include/logger.h"
#include "include/semaforo.h"
#include "include/shm.h"
#include <time.h>
#include <stdlib.h>
#ifndef N_PROCESOS
#define N_PROCESOS 5
#endif

int main(){
    srand(getpid());
    init_logger("Proceso de prueba", rand()%16);
    log_exit();
    
    int mutex = creasem(1);
    inisem(mutex,1); 

    int barrera = creasem(2);
    inisem(barrera,0);


    int memoryhndlr = creashm(1, sizeof(int));
    int* memory = (int*)map( memoryhndlr);
    *memory = N_PROCESOS;

    int continuar = 1;
    int proceso = 0;
    for(int i =1 ; i < N_PROCESOS && continuar; i++) {
        int pid = fork();
        continuar  =  pid > 0;
        if (continuar) {

            proceso = i;
            safelog("Inicia el proceso %d\n",proceso);    
        }
        else{
            srand(getpid());
            init_logger("Proceso de prueba", getpid());
            log_exit();
        }
        
    }
    if (proceso > 0)
     memory = (int*)map( memoryhndlr);

    usleep((rand() % 3) *1000);

    int despertar = 0;
    p(mutex);
        (*memory)-- ;
        despertar = (*memory == 0);
    v(mutex);
    


    safelog("El proceso %d llego\n",proceso);
    if(despertar) {
        safelog("El proceso %d levanta la barrera\n",proceso);
        for(int i = 0; i<N_PROCESOS; i++) v(barrera);
    }
    p(barrera);
    safelog("El proceso %d continua\n", proceso);


}
