
#include "include/middleware.h"
#include <csignal>

using namespace std;


int q_work;
int q_work_ret;

void signalHandler( int signum )
{
    safelog("Terminando Worker, liberando recursos\n");
    elimsg(q_work);
    elimsg(q_work_ret);
    exit(signum);

}


int main(int argc, char ** argsv){

    init_logger("MiddlewareWorker", getpid());
    log_exit();
    safelog("Iniciando Worker %s\n",argsv[0]);
    int fdConnection = atoi(argsv[1]);
    q_work = atoi(argsv[2]);
    q_work_ret = atoi(argsv[3]);
    signal(SIGINT,signalHandler);

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
        recibirmsg(q_work, &buf,MAX_BUFFER,0);
        safelog("Trabajo recibido, enviando hacia el cliente middleware\n");
        string work(buf.mtext);

        if(writeSocket(fdConnection, work)){
            //lo devuelvo a la cola y me voy si falla, asi no se pierde.
            enviarmsg(q_work,&buf,sizeof(long) + sizeof(char) * work.length());
            safelog("Error conectando con Client, finalizando Worker\n");
            exit(1);
        }

        //Esperando respuesta y reenvio al solicitante.
        safelog("Esperando traduccion\n");
        string trans = readSocket(fdConnection);
        safelog("Traduccion recibida, enviando hacia el cliente\n");
        strcpy(buf.mtext, trans.c_str());;
        enviarmsg(q_work_ret,&buf,sizeof(long) + sizeof(char) * trans.length());


    }


}
