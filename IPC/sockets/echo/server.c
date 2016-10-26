#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "errno.h"
#include "include/logger.h"
#include "include/connector.h"


#define EXIT_CODE 1
int main(int argc, char *argv[]) {
    init_logger("Servidor", getpid());
    log_exit();
	int fd = listenSocket(PORT);
	safelog("Iniciando servidor...\n");
	struct sockaddr_in cli_addr;
	int clilen = sizeof(cli_addr);
	safelog("Servidor escuchando pedidos en el puerto %d\n", PORT);
	int fdclient = accept(fd, (struct sockaddr *) &cli_addr, &clilen);
	if (fdclient < 0) {
		perror("ERROR haciendo accept\n");
		exit(1);
	}
	safelog("Cliente conectado\n",fdclient);
	char msj [MAX_BUFFER];
	readSocket(fdclient, msj);
	safelog("Se lee mensaje %s", msj);
	writeSocket(fdclient, msj);
	safelog("Finalizando servidor\n");
}
