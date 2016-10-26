#include "include/measure.h"
#include "include/logger.h"
#include "include/semaforo.h"
#include "include/shm.h"
#include <time.h>
#include <stdlib.h>
#ifndef N_CONSUMIDORES
#define N_CONSUMIDORES 5
#endif
#ifndef N_PRODUCTORES
#define N_PRODUCTORES 2
#endif
#define CANT_NUMEROS_PRODUCTOR 5

int main(){
    srand (getpid());
    init_logger("Proceso Starter", 0);
    log_exit();
    
    int lect = creasem(1);
    inisem(lect,0); 

    int escr = creasem(2);
    inisem(escr,1);

    //CREO LA MEMORIA COMPARTIDA [ARRAY DE VISTOS + EL NUMERO PRODUCIDO]
    int memoryhndlr = creashm(1, sizeof(int)*(N_CONSUMIDORES +1));
    int* memory = (int*)map( memoryhndlr);
    for(int i =0 ; i <= N_CONSUMIDORES;i++){
        memory[i] = 0;
    }
    
    //marco al proceso hijo
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



    if (productor || consumidor)
        memory = (int*)map( memoryhndlr);
    
    for (int j=0; j<CANT_NUMEROS_PRODUCTOR && productor;j++){
        usleep((rand() % 5) *1000);
        int gen = rand() % 100;
        //pido permiso de escritura
        p(escr);    
        safelog("El productor %d genera el numero %d\n",numeroproceso,gen);
        memory[N_CONSUMIDORES] = gen;
        //indico que nadie leyó mi numero
        for(int i = 0;i<N_CONSUMIDORES; i++) memory[i] =0;  
        //y abro la lectura.
        for (int i=0;i<N_CONSUMIDORES;i++) v(lect);

    }
    int leido = 0;
    int numerosconsumidos=0;
    while (numerosconsumidos< (CANT_NUMEROS_PRODUCTOR*N_PRODUCTORES) && consumidor) {
        usleep((rand() % 5) *1000);
        p(lect);
        leido = memory[N_CONSUMIDORES];
        if(memory[numeroproceso-1] == 0) {
            numerosconsumidos++;
           safelog("El consumidor %d, lee el numero %d\n", numeroproceso,leido);
           memory[numeroproceso-1] = 1;
           int consumidos = 0;
           for(int i =0; i< N_CONSUMIDORES; i++) consumidos+= memory[i];
            //si leyeron todos abro para que los escritores puedan leer.
            if(consumidos == N_CONSUMIDORES) v(escr);
        } else {
            safelog("El consumidor %d, lee nuevamente %d\n", numeroproceso,leido);
            v(lect);
        }
        

    }

    if (consumidor) safelog("Finalizando Consumidor %d \n", numeroproceso);
    if (productor) safelog("Finalizando Productor %d \n", numeroproceso);

}
