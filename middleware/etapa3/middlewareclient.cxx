
#include <unistd.h>
#include "include/logger.h"
#include "include/middleware.h"
#include <csignal>

int fdConnection;
int queueINFd;
int queueOUTFd;


void signalHandler( int signum )
{
    safelog("Terminando Client, liberando recursos\n");
    elimsg(queueINFd);
    elimsg(queueOUTFd);
    exit(signum);

}


int main(int argc, char ** argsv){

    init_logger("MiddlewareClient", getpid());
    log_exit();
    safelog("Iniciando MiddlewareClient %s\n", argsv[0]);
    fdConnection = atoi(argsv[1]);
    queueINFd = atoi(argsv[2]);
    queueOUTFd = atoi(argsv[3]);
    signal(SIGINT,signalHandler);

    safelog("Conectado al servidor\n");
    while(1) {
        //espero que me solicite un trabajo el cliente.
        mess_buff_t buf;
        memset(buf.mtext, 0, MAX_BUFFER);
        safelog("Esperando solicitud del traductor\n");
        recibirmsg(queueOUTFd, &buf,MAX_BUFFER,0);
        safelog("Solicitud de traductor recibida\n");
        // ahora pido trabajo al servidor:

        safelog("Solicitando trabajo al servidor\n");
        writeSocket(fdConnection,"1");
        string  work = readSocket(fdConnection);
        safelog("Trabajo del servidor recibido\n");
        //envio el trabajo al cliente:
        memset(buf.mtext, 0, MAX_BUFFER);
        strcpy(buf.mtext,work.c_str());
        safelog("Enviando trabajo al traductor\n");
        enviarmsg(queueINFd,&buf,sizeof(long) + sizeof(char) * work.length());

        //espero la respuesta:
        memset(buf.mtext, 0, MAX_BUFFER);
        safelog("Esperando traduccion desde el traductor\n");
        recibirmsg(queueOUTFd, &buf,MAX_BUFFER,0);
        string trans (buf.mtext);

        //envio la respuesta nuevamente al server
        writeSocket(fdConnection, trans);
        safelog("Traduccion devuelta al servidor\n");
    }
}
