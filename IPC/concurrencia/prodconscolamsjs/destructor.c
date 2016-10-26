#include "include/measure.h"
#include "include/semaforo.h"
#include "include/shm.h"
#include "include/msg.h"

#define ID_SEM1 1
#define ID_SEM2 2
#define ID_SHM1 1
#define ID_SHM2 2
#define ID_MSG 1

int main(){
    elisem(getsem(ID_SEM1));
    elisem(getsem(ID_SEM2));
   
    elishm(getshm(ID_SHM1));
    elishm(getshm(ID_SHM2));

    elimsg(getmsg(ID_MSG));
}
