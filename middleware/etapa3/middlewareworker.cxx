
#include "include/middleware.h"

using namespace std;
int main(int argc, char ** argsv){
    init_logger("MiddlewareWorker", getpid());
    log_exit();
    safelog("Iniciando Worker %s\n",argsv[0]);
    int fdConnection = atoi(argsv[1]);
    int fd_work = atoi(argsv[2]);
    int fd_work_ret = atoi(argsv[3]);

    safelog("Enviando informacion inicial al cliente\n");
    writeSocket(fdConnection,argsv[1]);
    while(1){
        safelog("Esperando solicitud del cliente\n");
        readSocket(fdConnection);
        safelog("Solicitud del cliente recibida\n");
        safelog("Esperando trabajo\n");
        mess_buff_t buf;
        buf.mtype = 0;
        memset(buf.mtext, 0, MAX_BUFFER);

        // ahora pido el trabajo.
        recibirmsg(fd_work, &buf,MAX_BUFFER,0);
        safelog("Trabajo recibido, enviando hacia el cliente middleware\n");
        string work(buf.mtext);

        if(writeSocket(fdConnection, work)){
            //lo devuelvo a la cola y me voy si falla, asi no se pierde.
            enviarmsg(fd_work,&buf,sizeof(long) + sizeof(char) * work.length());
            safelog("Error conectando con Client, finalizando Worker\n");
            exit(1);
        }

        //Esperando respuesta y reenvio al solicitante.
        safelog("Esperando traduccion\n");
        string trans = readSocket(fdConnection);
        safelog("Traduccion recibida, enviando hacia el cliente\n");
        strcpy(buf.mtext, trans.c_str());;
        enviarmsg(fd_work_ret,&buf,sizeof(long) + sizeof(char) * trans.length());


    }


}
