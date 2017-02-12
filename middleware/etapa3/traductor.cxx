
#include "include/logger.h"
#include "include/middleware.h"



//Translates a word
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
	//Log init
    init_logger("Traductor", getpid());
    log_exit();

    //Translator init
	safelog("Iniciando Traductor...\n");
	srand(getpid());
	string word;
	string result;

	while (1) {
		//Ask for a new task
		word = getTask();
		safelog("Trabajo recibido: %s\n", word.c_str());

		//random translation time
		sleep(rand()%10);

		//translate
		result = translate(word);
		safelog("Trabajo traducido a: %s\n", result.c_str());

		//returning result
        sendTask(result);
	}

}




