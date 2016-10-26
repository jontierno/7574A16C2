
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include <netinet/in.h>
#include <arpa/inet.h>



#define SENAL_DURANTE_CONEXION 4
#define PERMISO_DENEGADO 13
#define MAX_BUFFER_SOCK 100
void manejarWriteError(int error) {
	if(error == SENAL_DURANTE_CONEXION ) safelog("Se ha terminado la conexion durante el envio de datos");
	else {safelog("Error desconocido enviando %d\n", errno);	
	perror("Error enviando");}
	exit(1);
}

void manejarReadError(int error) {
	if(error == SENAL_DURANTE_CONEXION ) safelog("Se ha terminado la conexion durante la recepcion de datos\n");
	else {safelog("Error desconocido recibiendo %d\n", errno);	
	perror("Error leyendo");}
	exit(1);
}



void manejarConectError(int error) {
	if(error == PERMISO_DENEGADO) safelog("No se tiene permiso para la conexion, chequee el firewall\n");	
	else {safelog("Error desconocido al conectar %d\n", errno);	
	perror("Error conectando");}
	exit(1);
}


int connectSocket (char * ip, int port) {
	struct sockaddr_in dest_addr;
	safelog("Intentando conectar con servidor %s\n", ip);
	// Guardará la dirección de destino
	int socketfd = socket(AF_INET, SOCK_STREAM, 0); // ¡Comprueba errores!
	dest_addr.sin_family = AF_INET;
	// Ordenación de máquina
	dest_addr.sin_port = htons(port);
	// short, Ordenación de la red
	dest_addr.sin_addr.s_addr = inet_addr(ip);
	memset(&(dest_addr.sin_zero), '\0', 8); // Poner a cero el resto de la estructura
	int st = connect(socketfd, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr));
	if (st == -1) {
		manejarConectError(errno);
	}
	return socketfd;
}


int listenSocket(int port) {
	int fd1;
	struct sockaddr_in serv_addr;
	/* First call to socket() function */
	fd1 = socket(AF_INET, SOCK_STREAM, 0);
	if (fd1 < 0) {
		perror("ERROR abriendo socket.\n");
	}

	/* Initialize socket structure */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	memset(&(serv_addr.sin_zero), '\0', 8);
	int optval = 1 ;
	setsockopt(fd1,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int));
	/* Now bind the host address using bind() call.*/
	if (bind(fd1, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
         exit(1);
	}

	if (listen(fd1, 10) < 0){
		perror("ERROR on listening");
		exit(1);
	}
	return fd1;
}

void readSocket(int sock, char * msj) {

	char buffer[MAX_BUFFER_SOCK];
	bzero(buffer, 100);
	int longitudTotal;
	int recibidos = 0;
	int result;
	while (recibidos < MAX_BUFFER_SOCK) {
		result = recv(sock, &(buffer[recibidos]), MAX_BUFFER_SOCK - recibidos, MSG_NOSIGNAL);

		if (result == -1) {
			manejarWriteError(errno);
		}
		recibidos += result;
	}

	
    strcpy(msj,buffer);

}

void writeSocket(int socket , char * msj) {

	int enviados = 0;
	char sendline[MAX_BUFFER_SOCK];
	bzero(sendline, MAX_BUFFER_SOCK);
	int longEnviada;
	while (enviados < MAX_BUFFER_SOCK) {
		longEnviada = MAX_BUFFER_SOCK - enviados;
		//Ahora intento enviar lo restante.
		int result = send(socket, &(msj[enviados]), longEnviada, MSG_NOSIGNAL);
		//Si da error cancelo
		if (result == -1) {
			manejarWriteError(errno);
		}
		//Sumo a los enviados lo que envié.
		enviados += result;
	}
	
 }


