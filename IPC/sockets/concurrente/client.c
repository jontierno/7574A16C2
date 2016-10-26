#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "errno.h"
#include "include/logger.h"
#include "include/connector.h"


int main(int argc, char *argv[]) {
    init_logger("Cliente", getpid());
    log_exit();
	safelog("Iniciando Cliente...\n");
	int fdserver = connectSocket(SERVER_IP,SERVER_PORT);
	size_t	 n = MAX_BUFFER;
	char * sendline = malloc(n);
	safelog("Ingrese una linea no mayor a 100 caracteres\n");
	getline(&sendline, &n, stdin);
	writeSocket(fdserver, sendline);
	char rcvLine[MAX_BUFFER];
	readSocket(fdserver, rcvLine);
	safelog("El server contesta %s",rcvLine);
	close(fdserver);
	free (sendline);
}
