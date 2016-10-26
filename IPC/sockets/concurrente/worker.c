#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "errno.h"
#include "include/logger.h"
#include "include/connector.h"

int main(int argc, char *argv[]) {
    init_logger("Worker", getpid());
    int fdclient = atoi(argv[1]);
    srand(getpid());
    log_exit();

	safelog("Iniciando Worker...\n");
	
	char msj [MAX_BUFFER];
	readSocket(fdclient, msj);
	safelog("Se lee mensaje %s", msj);
	safelog("Esperando un tiempo aleatorio\n");
	sleep(rand()%30);
	writeSocket(fdclient, msj);	

}
