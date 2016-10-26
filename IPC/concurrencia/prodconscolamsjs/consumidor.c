#include "include/measure.h"
#include "include/logger.h"
#include "include/semaforo.h"
#include "include/shm.h"
#include "include/msg.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>


/**
    Este ejercicio modela el problema clásico de N procesos con M consumidores con un buffer circular de tamaño limitado
*/

//Tamaño del buffer.
#define LONG_BUFFER 10
//IDs  de semaforos usados-
#define ID_SEM1 1
#define ID_SEM2 2
//IDs de memorias compartidas usadas.
#define ID_SHM1 1

//ID de la cola
#define ID_MSG 1

#define MSJ_TYPE 1



typedef struct msgbuff
{
  long mtype;
  int num;
} mess_t;



int main(int argc, char *argv[]){
    

    int numeroproceso = atoi(argv[1]);
    srand (getpid());
    init_logger("Consumidor", getpid());
    log_exit();
    safelog("Iniciado el consumidor %d\n", numeroproceso);
    //Semaforo escritura
    int sem_esc = getsem(ID_SEM1);
    
    //Semaforo memoria compartida
    int sem_shm = getsem(ID_SEM2);

    //mapeo la memoria.
    int *contador = (int*) map( getshm(ID_SHM1));
    
    //obtengo la cola de mensajes.
    int msghndlr= getmsg(ID_MSG);



    int leido = 0;

    mess_t buf;


    // PROCESO CONSUMIDOR
    while (1) {
        
        usleep((rand() % 5) *100000);

        //leo mensaje de la cola
        recibirmsg(msghndlr, &buf, sizeof(mess_t),MSJ_TYPE);

        //actualizo el contador de mensajes.
        p(sem_shm);
        (*contador)--;
            
        safelog("CONSUMIDOR:%d LEE:%d BUFFER: %d LIBRES\n", numeroproceso, buf.num, LONG_BUFFER-*contador);
        // si libere el primer espacio abro la escritura.
        if(*contador == LONG_BUFFER - 1) {
            safelog("CONSUMIDOR %d ABRE ESCRITURA\n", numeroproceso);
            v(sem_esc);
        }
        v(sem_shm);

        
    }

    safelog("Finalizando Consumidor %d \n", numeroproceso);
    

}
