#include <string>
#include "msg.h"
#include "constants.h"
#include "msg.h"
#include <iostream>
using namespace std;

#define MAX_BUFFER 524288
#define MTYPE_REQUEST 1
#define MTYPE_RESPONSE 2

int fd_req = 0;
int fd_work = 0;

struct mess_buff_t
{
  long mtype;
  char mtext[MAX_BUFFER];
};


//Generates a new translation task
string toGeringoso(string word) {

	if(fd_req == 0) {
		fd_req = getmsg(QUEUEREQ_ID);
	}
	mess_buff_t buf;
    buf.mtype = MTYPE_REQUEST;
	memset(buf.mtext, 0, MAX_BUFFER);
    strcpy(buf.mtext,word.c_str());;
    enviarmsg(fd_req,&buf,sizeof(long) + sizeof(char) * word.length());
	memset(buf.mtext, 0, MAX_BUFFER);
    recibirmsg(fd_req, &buf,MAX_BUFFER,MTYPE_RESPONSE);
    string trans (buf.mtext);
    return trans;
}


//Ask for a new translation task
string getTask() {
	if(fd_work == 0) {
		fd_work = getmsg(QUEUEREQ_ID);
	}
	mess_buff_t buf;
	memset(buf.mtext, 0, MAX_BUFFER);
	recibirmsg(fd_work, &buf,MAX_BUFFER,MTYPE_REQUEST);
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
	buf.mtype = MTYPE_RESPONSE;
	memset(buf.mtext, 0, MAX_BUFFER);
	strcpy(buf.mtext, work.c_str());;
	enviarmsg(fd_work,&buf,sizeof(long) + sizeof(char) * work.length());
}


//Release translator resources
void finishWork(){
	
	if(fd_work != 0) close (fd_work);
	if(fd_req != 0) close (fd_req);


}