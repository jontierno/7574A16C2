#include <iostream>
#include "constants.h"
#include "cajero.h"

using namespace std;

void handleCrearCuenta(int nro) {

    int  result = crear(nro);
    if(result == CUENTA_YA_EXISTE) {
        printf("La cuenta %d ya existe\n", nro);
    }
    if(result == OPERACION_SUCCESS) {
        printf("Cuenta %d creada correctamente\n", nro);
    }


}


void handleConsultarSaldo(int nro) {

    int result = consultarSaldo(nro);

    if(result == CUENTA_INVALIDA) {
        printf("La cuenta %d no existe\n", nro);
    }
    if(result >= 0) {
        printf("La cuenta %d tiene un saldo de %d\n", nro, result);
    }

}


void handleDepositar(int nro, int cant) {
    int  result = depositar(nro, cant);

    if(result == CUENTA_INVALIDA) {
        printf("La cuenta %d no existe\n", nro);
    }
    if(result >= 0) {
        printf("La cuenta %d tiene un saldo de %d\n", nro, result);
    }
}

void handleRetirar(int nro, int cant) {

    int  result = retirar(nro, cant);

    if(result == CUENTA_INVALIDA) {
        printf("La cuenta %d no existe\n", nro);
    }

    if(result == SALDO_INSUFICIENTE) {
        printf("La cuenta %d no tiene saldo suficiente\n", nro);
    }

    if(result >= 0) {
        printf("La cuenta %d tiene un saldo de %d\n", nro, result);
    }
}


void handleMovimientos(int nro) {

    movimientos_t result = movimientos(nro);

    if(result.result== CUENTA_INVALIDA) {
        printf("La cuenta %d no existe\n", nro);
    }

    if(result.result == OPERACION_SUCCESS) {
        for(int i = 0; i < result.movs.size(); i++) {
            std::string str(result.movs[i].tipo == OP_DEPOSITO ? "DEP" : "SUS");
            printf("Operacion: %s              Cantidad: %d     \n",str.c_str(), result.movs[i].cantidad);
        }
    }
}



int  main (int argc, char *argv[])
{


    int cuenta;
    int operacion;
    printf("Ingrese cuenta: \n");
    scanf("%d",&cuenta);
    printf("Ingrese operacion CREAR(1), CONSULTA(2), DEPOSITAR(3), RETIRAR(4), MOVIMIENTOS(5): \n");
    scanf("%d",&operacion);

    int cant;
    if(operacion == 3 || operacion == 4){
        printf("Ingrese cantidad: \n");
        scanf("%d",&cant);

    }


    switch (operacion) {
        case 1:
            handleCrearCuenta(cuenta);
            break;
        case 2:
            handleConsultarSaldo(cuenta);
            break;
        case 3:
            handleDepositar(cuenta,cant);
            break;
        case 4:
            handleRetirar(cuenta,cant);
            break;
        case 5:
            handleMovimientos(cuenta);
            break;
        default:
            break;
    }


    return 0;
}