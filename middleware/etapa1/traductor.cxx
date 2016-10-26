
#include "include/logger.h"
#include "include/middle.h"



string translate(string word) {
	string result = "";
	for (int i =  0; i < word.size() ; i++){
		result += word.at(i);
		if (strchr("aeiouAEIOUáéíóúÁÉÍÓÚ", word.at(i))){
			result += "p";
			result += word.at(i);
		}
	}

	return result;
}

int main(int argc, char *argv[]) {
    init_logger("Traductor", getpid());
    log_exit();
	safelog("Iniciando Traductor...\n");
	srand(getpid());
	string word;
	string result;
	while (1) {
		word = getWork();
		safelog("Trabajo recibido: %s\n", word.c_str());
		sleep(rand()%10);
		result = translate(word);
		safelog("Trabajo traducido a: %s\n", result.c_str());
        sendWork(result);
	}

}




