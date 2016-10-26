//
// Created by jonathan on 12/10/16.
//

#ifndef TRADICIONAL_CAJERO_H
#define TRADICIONAL_CAJERO_H

#endif //TRADICIONAL_CAJERO_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

typedef struct {
    int tipo;
    int cantidad;
} movimiento_t;


typedef struct {
    int result;
    std::vector<movimiento_t> movs;
} movimientos_t;


string mainName (int cnumber) {
    char str[15];
    sprintf(str, "%d", cnumber);
    string name(CUENTA_ARCH);
    name += str;
    return name;
}

string movName (int cnumber) {
    char str[15];
    sprintf(str, "%d", cnumber);
    string name(CUENTA_MOV);
    name += str;
    return name;
}


int consultarSaldo(int nro){
    int  result;
    string fileName = mainName(nro);
    ifstream infile(fileName);
    if (!infile.good()){
        return CUENTA_INVALIDA;
    }
    char cadena[128];
    infile >> cadena;

    infile.close();
    result = atoi(cadena) ;
    return result;
}

int depositar(int nro, int cant){
    string fileName = mainName(nro);
    ifstream infile(fileName);
    if (!infile.good()){
        return CUENTA_INVALIDA;
    }

    //leo el saldo anterior y calculo el nuevo saldo.
    char cadena[128];
    infile >> cadena;
    infile.close();
    int saldo = atoi(cadena) ;
    int saldoAct = saldo + cant;

    //actualizo los archivos
    ofstream o(fileName);
    o << saldoAct << endl;
    o.close();

    fileName =movName(nro);
    std::ofstream ofs;
    ofs.open (fileName, std::ofstream::out | std::ofstream::app);
    ofs << OP_DEPOSITO << ";"<< cant <<";"<< saldoAct << endl;
    ofs.close();

    //retorno el saldo actualizado
    return saldoAct;
}

int retirar(int nro, int cant){
    string fileName = mainName(nro);
    ifstream infile(fileName);
    if (!infile.good()){
        return CUENTA_INVALIDA;
    }
    char cadena[128];
    infile >> cadena;
    infile.close();
    int saldo = atoi(cadena) ;
    if(saldo < cant) {
        return SALDO_INSUFICIENTE;
    }
    int saldoAct = saldo - cant;


    ofstream o(fileName);
    o << saldoAct << endl;
    o.close();

    fileName =movName(nro);
    std::ofstream ofs;
    ofs.open (fileName, std::ofstream::out | std::ofstream::app);
    ofs << OP_EXTRACCION << ";"<< cant <<";"<< saldoAct << endl;
    ofs.close();
    return saldoAct;
}


movimientos_t movimientos(int nro){
    movimientos_t  result;
    result.result = OPERACION_SUCCESS;
    // no performante, solo ejemplo.
    string fileName =movName(nro);
    ifstream infile (fileName);
    if (!infile.good()){
        result.result = CUENTA_INVALIDA;
        return result;
    }
    int op, saldo, saldoact;



    movimiento_t movimientos[MOVIMIENTOSC];
    string line;
    int count = 0;
    int i = 0;
    while ( getline (infile,line) ) {
        stringstream  lineStream(line);
        string  cell;
        getline (lineStream,cell, ';');
        movimientos[i].tipo = atoi(cell.c_str());
        getline (lineStream,cell, ';');
        movimientos[i].cantidad= atoi(cell.c_str());
        i = (i+1)%MOVIMIENTOSC;
        count++;
    }



    count = count > 10 ? 10 : count;

    //ahora ordeno y lo paso al result.
    int aux = i;
    while (count > 0) {
        aux = aux - 1;
        aux = aux < 0 ? MOVIMIENTOSC-1: aux;
        result.movs.push_back(movimientos[aux]);
        count --;
    }


    return result;

}



int crear (int nro) {
    if(nro < 0 ) {
        return CUENTA_INVALIDA;
    }
    string fileName = mainName(nro);
    ifstream infile(fileName);
    if (infile.good()){
        return CUENTA_YA_EXISTE;
    }

    ofstream o(fileName);
    o << 0<< endl;
    o.close();
    fileName = movName(nro);
    ofstream oc(fileName);
    oc.close();

    return OPERACION_SUCCESS;
}





