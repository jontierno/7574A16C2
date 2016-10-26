#include "include/measure.h"
#include "include/semaforo.h"
#include "include/shm.h"

#define ID_SEM1 1
#define ID_SEM2 2
#define ID_SEM3 3
#define ID_SHM1 1
#define ID_SHM2 2

int main(){
    elisem(getsem(ID_SEM1));
    elisem(getsem(ID_SEM2));
    elisem(getsem(ID_SEM3));

    
    elishm(getshm(ID_SHM1));
    elishm(getshm(ID_SHM2));
}
