#include <string>
#include <iostream>
#include "include/logger.h"
#include "include/middleware.h"
using namespace std;

int main(int argc, char *argv[]) {
    //Logger init
    init_logger("Cliente", getpid());
    log_exit();
	safelog("Iniciando cliente...\n");

	string word;
	string translation;
	
	while(1) {
			//Word input
			safelog("Ingrese una palabra: ");
			getline(cin, word);

			//Translate
   			translation = toGeringoso(word);

   			//Answer translation
			safelog("Su palabra en jeringoso es: %s\n", translation.c_str());
	}

}
