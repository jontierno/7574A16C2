#include <string>
#include "msg.h"
#include "constants.h"
#include "msg.h"
#include <iostream>
using namespace std;

#define MAX_BUFFER 524288

int fd_req = 0;
int fd_ret = 0;

int fd_work = 0;
int fd_work_ret = 0;
int current_work_id = 0;


struct mess_buff_t
{
  long mtype;
  char mtext[MAX_BUFFER];
};


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


string getTask() {
	if (current_work_id != 0) {
		cout << "Pinchar, ver que hacer"<< endl;
		exit(1);
	}
	if(fd_work == 0) {
		fd_work = getmsg(QUEUEREQ_ID);
		fd_work_ret = getmsg(QUEUERET_ID);
	}
	mess_buff_t buf;
	memset(buf.mtext, 0, MAX_BUFFER);
	recibirmsg(fd_work, &buf,MAX_BUFFER,0);
	current_work_id = buf.mtype;
	string work(buf.mtext);
	return work;

}


void sendTask(string work) {
	if (fd_work == 0) {
		cout << "Pincha, ver que hacer"<<endl;
		exit(1);
	}
	mess_buff_t buf;
	buf.mtype = current_work_id;
	memset(buf.mtext, 0, MAX_BUFFER);
	strcpy(buf.mtext, work.c_str());;
	current_work_id = 0;

	enviarmsg(fd_work_ret,&buf,sizeof(long) + sizeof(char) * work.length());
}


void finishWork(){
	
	if(fd_work != 0) close (fd_work);
	if(fd_work_ret != 0) close (fd_work_ret);
	if(fd_req != 0) close (fd_req);
	if(fd_ret != 0) close (fd_ret);

}