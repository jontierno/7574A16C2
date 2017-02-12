#include <string>
#include "msg.h"
#include "constants.h"
#include "msg.h"
#include <iostream>
#include <vector>
#include "utils.h"
#include "connector.h"
#include <fstream>

using namespace std;

#define MAX_BUFFER 524288

int fd_req = 0;
int fd_ret = 0;


int connected = 0;

int fd_work = 0;
int fd_work_ret = 0;


struct mess_buff_t
{
  long mtype;
  char mtext[MAX_BUFFER];
};


//Generates a new translation task
string toGeringoso(string word) {

	if(fd_req == 0) {
		fd_req = getmsg(QUEUEREQ_ID);
		fd_ret = getmsg(QUEUERET_ID);
	}
	mess_buff_t buf;
    buf.mtype = getpid();
	memset(buf.mtext, 0, MAX_BUFFER);
    strcpy(buf.mtext,word.c_str());;
    enviarmsg(fd_req,&buf,sizeof(long) + sizeof(char) * word.length());
	memset(buf.mtext, 0, MAX_BUFFER);
    recibirmsg(fd_ret, &buf,MAX_BUFFER,buf.mtype);
    string trans (buf.mtext);
    return trans;
}



void runClientWorker(int number, int connectionFd,int queueINFd, int queueOUTFd) {




	vector<char *> av;
	av.push_back(const_cast<char *> (ITOS(number).c_str()));
	av.push_back(const_cast<char *> (ITOS(connectionFd).c_str()));
	av.push_back(const_cast<char *> (ITOS(queueINFd).c_str()));
	av.push_back(const_cast<char *> (ITOS(queueOUTFd).c_str()));
	av.push_back(0);
	int pid = fork();
	if (pid == 0) {
		//run worker
		execlp(CLIENT_WORKER_BIN, ITOS(number).c_str(), ITOS(connectionFd).c_str(), ITOS(queueINFd).c_str(), ITOS(queueOUTFd).c_str(), 0);

	}
}

string readIPserver(){
	string line;
	ifstream myfile (SETTINGS_FILE);
	if (myfile.is_open()){
		if(!getline (myfile,line)){
			safelog("Error leyendo configuración");
			exit(1);
		}
		myfile.close();
		safelog("Ip Leida %s\n", line.c_str());
		return line;
	} else {
		safelog("Error abriendo archivo");
		exit(1);
	}

}

//Ask for a new translation task
string getTask() {

	if(connected == 0) {
		string serverIP = readIPserver();
		// connects to server and ask translator number
		int fdClient = connectSocket(serverIP.c_str(), SERVER_PORT);

		// reads client number
		string number = readSocket(fdClient);

		int controlNumber = atoi(number.c_str());

		int queueINID =controlNumber + CLIENT_QUEUE_OFFSET;
		int queueOUTID =queueINID + CLIENT_QUEUE_SEPARATOR;
		// creates messagge queues
		fd_work = creamsg(queueINID);
		fd_work_ret = creamsg(queueOUTID);
		connected = 1;

		// runs client's worker and gives it fds
		safelog("Se lanza un Client MiddleWare\n");
		runClientWorker(controlNumber, fdClient, fd_work, fd_work_ret);

		//closes socket
		close(fdClient);
	}

	mess_buff_t buf;

	//sends an empty message as a task request
	memset(buf.mtext, 0, MAX_BUFFER);
    strcpy(buf.mtext, "1");
	enviarmsg(fd_work_ret,&buf, sizeof(long) + sizeof(char));

	//there is a new task, asks for it
    memset(buf.mtext, 0, MAX_BUFFER);
    buf.mtype = 0;
	recibirmsg(fd_work, &buf,MAX_BUFFER,0);
	string work(buf.mtext);
	return work;

}

//Answer last translation task
void sendTask(string work) {
	if (fd_work == 0) {
		cout << "Pincha, ver que hacer"<<endl;
		exit(1);
	}
	mess_buff_t buf;
	buf.mtype = 1;
	memset(buf.mtext, 0, MAX_BUFFER);
	strcpy(buf.mtext, work.c_str());
	enviarmsg(fd_work_ret,&buf,sizeof(long) + sizeof(char) * work.length());
}

//Release translator resources
void finishWork(){
	
	if(fd_work != 0) close (fd_work);
	if(fd_work_ret != 0) close (fd_work_ret);
	if(fd_req != 0) close (fd_req);
	if(fd_ret != 0) close (fd_ret);

}