#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

#define SIGNAL_WHILE_CONNECTION 4
#define DENIED_ACCESS 13
#define MAX_BUFFER_SOCK 524288

void handleWriteError(int error) {
	if(error == SIGNAL_WHILE_CONNECTION ) safelog("Se ha terminado la conexion durante el envio de datos");
	else safelog("Error desconocido enviando %d\n", errno);
	//perror("Error enviando");
}

void handleReadError(int error) {
	if(error == SIGNAL_WHILE_CONNECTION ) safelog("Se ha terminado la conexion durante la recepcion de datos\n");
	else {safelog("Error desconocido recibiendo %d\n", errno);	
	perror("Error leyendo");}
	exit(1);
}


void handleConnectError(int error) {
	if(error == DENIED_ACCESS) safelog("No se tiene permiso para la conexion, chequee el firewall\n");
	else {safelog("Error desconocido al conectar %d\n", errno);	
	perror("Error conectando");}
	exit(1);
}



/* Conecta a un socket y retorna su file descriptor
   ip: dirección ip destino
   port: puerto destino
*/
int connectSocket (const char * ip, int port) {
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
		handleConnectError(errno);
	}
	return socketfd;
}

/* Abre la espera para conexciones
   port: puerto destino
*/
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


/* Lee una cadena de caracteres,
   sock: file descriptor del socket
   retorna el mensaje leido
*/
string readSocket(int sock) {

	//	string buffer;
	char buffer[MAX_BUFFER_SOCK] = "";
	int totalLength;
	int received = 0;
	int result = recv(sock, &totalLength, sizeof(int), MSG_NOSIGNAL);
	if (result == -1) {
		handleReadError(errno);
	}

	while (received < totalLength) {
		result = recv(sock, &(buffer[received]), totalLength - received, MSG_NOSIGNAL);
		if (result == -1) {
			handleReadError(errno);
		}
		received += result;
	}
	string resultstr(buffer);
	return resultstr;
}


/* Envía una cadena de caracteres,
   sock: file descriptor del socket
   msj:  mensaje a enviar
*/
int writeSocket(int socket , string msj) {

	int sent = 0;
	int len = msj.length();
	int lengthSent;
	const char * msj_c = msj.c_str();
	if (len <= MAX_BUFFER_SOCK) {
		//envio longitud total
		int result = send(socket, &len, sizeof(int), MSG_NOSIGNAL);
		if (result == -1) {
			handleWriteError(errno);
			return -1;
		}
		//envio el mensaje, todo lo que entre en el buffer del SO
		while (sent < len) {
			lengthSent = len - sent;
			//Ahora intento enviar lo restante.
			result = send(socket, &(msj_c[sent]), lengthSent, MSG_NOSIGNAL);
			//Si da error cancelo
			if (result == -1) {
				handleWriteError(errno);
				return -1;
			}
			//Sumo a los enviados lo que envié.
			sent += result;
		}
	} else {
		safelog("Mensaje demasiado largo\n");

	}
	return 0;
 }

