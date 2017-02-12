#include "include/msg.h"
#include "include/constants.h"

int main(){
    init_logger("Middleware", getpid());
    log_exit();
    safelog("Iniciando recursos compartidos\n");
    creamsg(QUEUEREQ_ID);

    safelog("Recursos iniciados correctamente\n");
}
