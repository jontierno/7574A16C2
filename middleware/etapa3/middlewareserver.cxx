#include "include/msg.h"
#include "include/constants.h"
#include "include/connector.h"
#include <vector>
#include "include/utils.h"
#include <csignal>
#include <sys/wait.h>

using namespace std;
int fd_work;
int fd_work_ret;
int fd_server;
void signalHandler( int signum )
{
    safelog("Eliminando recursos, senial %d recibida\n", signum);
    elimsg(fd_work);
    elimsg(fd_work_ret);
    close(fd_server);
    exit(signum);
}

void sigchld_handler(int sign) {
    int local_errno = errno; // backup errno,

    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;

    errno = local_errno; // restore errno,
}

void runWorker(int number, int connectionFd, int fdClient, int fd_work, int fd_work_ret) {

    static vector<char *> av;
    av.clear();

    av.push_back(const_cast<char *> (WORKER_BIN));
    av.push_back(const_cast<char *> (ITOS(number).c_str()));
    av.push_back(const_cast<char *> (ITOS(fdClient).c_str()));
    av.push_back(const_cast<char *> (ITOS(fd_work).c_str()));
    av.push_back(const_cast<char *> (ITOS(fd_work_ret).c_str()));
    std::cout << &(av[0]) << std::endl;
    av.push_back(0);
    int pid = fork();
    if (pid == 0) {
        //el hijo cierra el fd que hizo el accept.
        close(connectionFd);
        
        execlp(WORKER_BIN, ITOS(number).c_str(), ITOS(fdClient).c_str(), ITOS(fd_work).c_str(), ITOS(fd_work_ret).c_str(), 0);
        safelog("%s\n", strerror(errno));
        exit(1);

        //el padre cierra el fd del cliente.
        close(fdClient);
    }
}



int main(){
    init_logger("Middleware", getpid());
    log_exit();
    safelog("Iniciando recursos compartidos\n");
    fd_work = creamsg(QUEUEREQ_ID);
    if(fd_work == -1) {
        fd_work = getmsg(QUEUEREQ_ID);
    }
    fd_work_ret = creamsg(QUEUERET_ID);
    if(fd_work_ret == -1) {
        fd_work_ret = getmsg(QUEUEREQ_ID);
    }
    signal(SIGINT, signalHandler);
    signal(SIGCHLD, sigchld_handler);
    fd_server = listenSocket(SERVER_PORT);
    safelog("Iniciando servidor...\n");
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    safelog("Servidor esperando traductores en el puerto %d\n", SERVER_PORT);
    safelog("Recursos iniciados correctamente\n");
    int clientID = 0;
    while(1) {
        int fdclient = accept(fd_server, (struct sockaddr *) &cli_addr, &clilen);
        if (fdclient < 0) {
            perror("ERROR haciendo accept\n");
            exit(1);
        }
        safelog("Traductor conectado\n");
        // lanzo un worker
        runWorker(clientID,fd_server,fdclient,fd_work, fd_work_ret);
        clientID+=2;
  }

}
