#include <string>
#include <iostream>
#include "include/logger.h"
#include "include/middleware.h"
using namespace std;

int main(int argc, char *argv[]) {
    init_logger("Productor", getpid());
    srand(getpid());
    log_exit();
	safelog("Iniciando cliente...\n");
	string palabra;
	string traduccion;
	while(1) {
			safelog("Ingrese una palabra: ");
			getline(cin, palabra);
   			traduccion = toGeringoso(palabra);
		safelog("Su palabra en jeringoso es: %s\n", traduccion.c_str());
	}

}
