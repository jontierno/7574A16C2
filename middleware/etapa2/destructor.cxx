#include "include/msg.h"
#include "include/constants.h"


int main(){
    init_logger("Liberador", getpid());
    log_exit();
    safelog("Liberando recursos compartidos\n");
    elimsg(getmsg(QUEUEREQ_ID));
    elimsg(getmsg(QUEUERET_ID));
    safelog("Recursos compartidos liberados correctamente\n");
}
