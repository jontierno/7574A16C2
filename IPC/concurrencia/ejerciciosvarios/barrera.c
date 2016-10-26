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
    init_logger("Proceso de prueba", KGRN);
    log_exit();
    int mutex1 = creasem(1);
    inisem(mutex1,0);   
    int mutex2 = creasem(2);
    inisem(mutex2,0);
    int isRunner = 0;
    int processNumber = 0;

    for(int i =1 ; i<= N_PROCESOS; i++) {
        int pid = fork();
        if(pid == 0) {
            srand(getpid());
            init_logger("Runer", getpid());
            isRunner =1;
            processNumber = i;
            break;    
        }
    }

    if (isRunner) {
        usleep((rand() % 3) *1000);
        safelog("El proceso %i llego\n",processNumber);
        v(mutex2);
        p(mutex1);
        safelog("El proceso %i finalizo\n",processNumber);
    } else {
        //espero los procesos
        safelog("Espero todos los procesos\n");
        for(int i =0; i < N_PROCESOS; i++){
            p(mutex2);
        }
        //libero los procesos
        safelog("Libero la barrera\n");
        for(int i =0; i < N_PROCESOS; i++){
            v(mutex1);
        }
        elisem(getsem(1));
        elisem(getsem(2));
    }

}
