#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "errno.h"
#include "include/logger.h"
#include "include/connector.h"
#include "include/constants.h"

int main(int argc, char *argv[]) {
    init_logger("Servidor", getpid());
    srand(getpid());
    log_exit();
	int fd = listenSocket(SERVER_PORT);
	safelog("Iniciando servidor...\n");
	struct sockaddr_in cli_addr;
	int clilen = sizeof(cli_addr);
	safelog("Servidor escuchando pedidos en el puerto %d\n", SERVER_PORT);

	while(1) {
		int fdclient = accept(fd, (struct sockaddr *) &cli_addr, &clilen);
		if (fdclient < 0) {
			perror("ERROR haciendo accept\n");
			exit(1);
		}
		safelog("Cliente conectado\n");
        
		// lanzo un worker
        char buffer[10];    
        snprintf(buffer, 10, "%d", fdclient);
        char * args[] = {WORKER_BIN, buffer, NULL};
        int pid = fork();
        if (pid == 0) {
        	//el hijo cierra el fd que hizo el accept.
        	close(fd);
        	//ejecuto el worker
            if (execv(WORKER_BIN, args) != 0){
                safelog("%s\n",strerror(errno));
                exit(1);
	        }
	        //el padre cierra el fd del cliente.
	        close(fdclient);	           
	    }

	}

}
