#include "include/measure.h"
#include "include/logger.h"
#include "include/semaforo.h"
#include "include/shm.h"
#include <time.h>
#include <stdlib.h>


int main(){
    srand(time(NULL));
    init_logger("Proceso de prueba", KGRN);
    log_exit();
    int mutex = creasem(1);
    inisem(mutex,0); 

    int memoryhndlr = creashm(1, sizeof(int));
    int pid = fork();
    if(pid == 0) {
        // productor
        int* memory = (int*)map( memoryhndlr);
        int generated = rand();
        safelog("Generado %i\n",generated);
        
         *memory = generated;
        v(mutex);
    } else {
        //consumidor
        int* memory = map( memoryhndlr);
        p(mutex);
        safelog("Consumido %i\n",*memory);
        v(mutex);

    }
//    tic();

//    usleep(500*1000);
//    tic();

//    usleep(500*1000);
//    tic();
    
//    safelog("%f\n", toc()); // 0
//    safelog("%f\n", toc()); // 500
//    tic();
//    usleep(200*1000);
//    safelog("%f\n", toc()); //200
//  tic(); 
//    usleep(100*1000);

//    tic();

//    usleep(200*1000);
//    tic();
//    safelog("%f\n", toc()); //0

//    safelog("%f\n", toc()); //200

//    safelog("%f\n", toc()); //300
//    safelog("%f\n", toc()); //15000
}
