#ifndef _MSG_H_
#define _MSG_H_

#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "logger.h"
#include "resources.h"


/*Crea una cola de mensajes
  id: Id de la cola de mensajes
  */
int creamsg(int id){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    return (msgget(clave,  IPC_CREAT | IPC_EXCL | 0660));
    /* da error si ya existe */
}

/*Obtiene una cola de mensajes
  id: Id de la cola de mensajes
  */
int getmsg(int id){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    return (msgget(clave, 0660));
}


/*Envía un mensaje por la cola de mensajes
  id: Id de la cola de mensajes
  msgp: un puntero al mensaje
  msgsz: tamaño del mensaje en bytes
  */

void enviarmsg(int id, const void *msgp, size_t msgsz){
    if(msgsnd(id,msgp,msgsz-sizeof(long),0)==-1){
        safeperror("No se puede enviar el mensaje");
        exit(-1);
    }
}



/*Recibe mensaje por la cola de mensajes
  id: Id de la cola de mensajes
  msgp: un puntero dnde se almacenará el mensaje
  msgsz: tamaño del mensaje en bytes
  type: tipo de mensaje
  */
void recibirmsg(int id, void *msgp, size_t msgsz, long type){
    if(msgrcv(id,msgp,msgsz-sizeof(long),type,0)==-1){
        safeperror("No se puede recibir el mensaje");
        exit(-1);
    }
}


/*Elimina una cola de mensajes
  id: Id de la cola de mensajes
  */
int   elimsg(int id){
    return (msgctl(id, IPC_RMID, NULL));
}

#endif /* _MSG_H_ */
