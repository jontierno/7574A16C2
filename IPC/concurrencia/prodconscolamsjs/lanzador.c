#include "include/measure.h"
#include "include/logger.h"
#include "include/semaforo.h"
#include "include/shm.h"
#include "include/msg.h"
#include <time.h>
#include <errno.h>
#include <sys/wait.h>


/**
    Este ejercicio modela el problema clásico de N productores con M consumidores 
    con un buffer de tamaño limitado utilizando cola de mensajes y semáforos
*/

/**

################################################################################
                                _______
                               |       |
                               |  SHM  |
                               |_______|
                                  ___
                                 |(o)|
                                 |   |
                                 |(o)|
                                 |___|

                                 ^  ^
    ______                       ║  ║
  _(      )_ ====================   ================
 (_  CONS  _)                              ___      ║      ________  
   (______)  ======                       |(o)|      === _(        )_
                   ║     ______________   |   |   <==== (_   PROD   _)
                    =>  |__|__|__|__|__|  |(o)|           (________)
                                          |___|

###############################################################################

*/

// Cantidad de consumidores
#define N_CONSUMIDORES 3
// Cantidad de productores
#define N_PRODUCTORES 3
//Tamaño del buffer circular
#define LONG_BUFFER 10
//IDs  de semaforos usados-
#define ID_SEM1 1
#define ID_SEM2 2

//IDs de memorias compartidas usadas.
#define ID_SHM1 1

//ID de la cola
#define ID_MSG 1

//nombres de los binarios de los productores y consumidores
#define PRODUCTOR_BIN "./productor"
#define CONSUMIDOR_BIN "./consumidor"



int main(){
    srand (getpid());
    init_logger("Proceso Starter", 0);
    log_exit();
    
  

    //Semaforo escritura
    int sem_esc = creasem(ID_SEM1);
    inisem(sem_esc,1);

    //Semaforo memoria compartida
    int sem_shm = creasem(ID_SEM2);
    inisem(sem_shm,1);

    //CREO LA MEMORIA COMPARTIDA para guardar la cantidad de elementos en la cola
    int memoryhndlr = creashm(ID_SHM1, sizeof(int)*LONG_BUFFER);
    int* contador = (int*)map(memoryhndlr);
    *contador = 0;

    //CREO LA COLA DE MSG.
    creamsg (ID_MSG);

    
    //ejecuto los consumidores
    for(int i =1; i <= N_CONSUMIDORES; i++) {
        char buffer[10];    
        snprintf(buffer, 10, "%d", i);
        char *args[] = {CONSUMIDOR_BIN, buffer, NULL};
        args[1] = strdup (buffer);
        int pid = fork();
        if (pid == 0) {

            if (execv(CONSUMIDOR_BIN, args) == -1 ){
                safelog("%s\n",strerror(errno));
                exit(1);
            }           
        }      
    }

    //EJECUTO LOS PRODUCTORES

    for(int i =1; i <= N_PRODUCTORES; i++) {
        char buffer[10];    
        snprintf(buffer, 10, "%d", i);
        char *args[] = {PRODUCTOR_BIN, buffer, NULL};
	args[1] = strdup (buffer);
        int pid = fork();
        if (pid == 0) {    
            if (execv(PRODUCTOR_BIN, args) == -1 ){
                safelog("%s\n",strerror(errno));
                exit(1);
            } 
        }        
    }

}
