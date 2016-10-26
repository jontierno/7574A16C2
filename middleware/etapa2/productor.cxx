#include <string>
#include <iostream>
#include "include/logger.h"
#include "include/middle.h"
using namespace std;

int main(int argc, char *argv[]) {
    init_logger("Productor", getpid());
    srand(getpid());
    log_exit();
	safelog("Iniciando productor...\n");
	string palabra;
	string traduccion;
	while(1) {
			safelog("Ingrese una palabra: ");

   			cin >> palabra; 
   			traduccion = toGeringoso(palabra);
		safelog("Su palabra en jeringoso es: %s\n", traduccion.c_str());
	}

}
