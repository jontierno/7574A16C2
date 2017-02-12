#include <string>
#include <iostream>
#include "include/logger.h"
#include "include/middleware.h"
using namespace std;

int main(int argc, char *argv[]) {
    //Logger init
    init_logger("Productor", getpid());
    log_exit();
	safelog("Iniciando cliente...\n");

	string palabra;
	string traduccion;
	
	while(1) {
			//Word input
			safelog("Ingrese una palabra: ");
			getline(cin, palabra);

			//Translate
   			traduccion = toGeringoso(palabra);

   			//Answer translation
			safelog("Su palabra en jeringoso es: %s\n", traduccion.c_str());
	}

}
