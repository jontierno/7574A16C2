#include <iostream>
#include <stdlib.h>
#include "constants.h"
#include "cajero.h"
#include "string"
#include "vector"
#include <stdio.h>
#include <cstring>

using namespace std;


string handleCrearCuenta(int nro) {

    int  result = crear(nro);
    char buf[100];
    if(result == CUENTA_YA_EXISTE) {
        sprintf(buf,"{ \"status\": \"La cuenta %d ya existe\"}", nro);

    }
    if(result == OPERACION_SUCCESS) {
        sprintf(buf,"{ \"saldo\": %d}", 0);
    }

    return string(buf);
}


string handleConsultarSaldo(int nro) {

    int result = consultarSaldo(nro);
    char buf[100];
    if(result == CUENTA_INVALIDA) {
        sprintf(buf, "{\"status\": \"La cuenta %d no existe\"}", nro);
    }
    if(result >= 0) {
        sprintf(buf,"{\"saldo\": %d}", result);      
    }
    return string(buf);

}


string handleDepositar(int nro, int cant) {
    int  result = depositar(nro, cant);
    char buf[100];
    if(result == CUENTA_INVALIDA) {
        sprintf(buf, "{\"status\": \"La cuenta %d no existe\"}", nro);
    }
    if(result >= 0) {
        sprintf(buf,"{\"saldo\": %d}", result);   
    }
    return string(buf);
}

string handleRetirar(int nro, int cant) {

    int  result = retirar(nro, cant);
    char buf[100];
    if(result == CUENTA_INVALIDA) {
        sprintf(buf, "{\"status\": \"La cuenta %d no existe\"}", nro);
    }

    if(result == SALDO_INSUFICIENTE) {
        sprintf(buf, "{\"status\": \"La cuenta %d no tiene saldo suficiente\"}", nro);
    }

    if(result >= 0) {
        sprintf(buf,"{\"saldo\": %d}", result);   
    }
    return string(buf);
}


string handleMovimientos(int nro) {

    movimientos_t result = movimientos(nro);
    if(result.result== CUENTA_INVALIDA) {
        char buf[50];
        sprintf(buf, "{\"status\": \"La cuenta %d no existe\"}", nro);
        return string(buf);
    }

    if(result.result == OPERACION_SUCCESS) {

        string res;
        for(int i = 0; i < result.movs.size(); i++) {
            std::string str(result.movs[i].tipo == OP_DEPOSITO ? "DEP" : "SUS");
            if(res.length()>0){
                res += ",";
            }
            char buf[50];
            sprintf(buf,"{ \"operacion\": \"%s\", \"cantidad\":%d}",str.c_str(), result.movs[i].cantidad);
            res += string(buf);
        }
        res  = "{\"movimientos\":[" +res + "]}";
        return res;
    }
    return "";
}




vector<string> split(const string str, const string& delim){
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) {
            tokens.push_back(token);
        }
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

vector<int> parseOperation (string str) {
    vector<int> numbers;
    vector <string> spl = split(str, ";");
    int n;
    for (vector<string>::iterator it = spl.begin(); it != spl.end(); ++it) {
            sscanf((*it).c_str() , "%d", &n);
            numbers.push_back(n);
    }
    return numbers;
}

int  main (int argc, char *argv[])
{
    char * query  = getenv("QUERY_STRING");
    string method(getenv("REQUEST_METHOD"));
    string script(getenv("SCRIPT_NAME"));
    printf("Content-type: application/json\n");

    vector<string> sp = split(script, "/");
    string op = sp[sp.size()-1];
    
    int error = 1;
    string result;
    if (op == "cuenta") {

        if(method == "GET" && query != NULL) {
            int numero;
            sscanf(query , "%d", &numero);
            result = handleConsultarSaldo(numero);
            error = 0;
        }
       if(method == "POST") {
            int body;
            cin >> body;
            result = handleCrearCuenta(body);
            error = 0;
        }
        
    }

    if (op == "movimientos") {
        if(method == "GET" && query != NULL) {
            int numero;
            sscanf(query , "%d", &numero);
            result = handleMovimientos(numero);
            error = 0;
        }
    }

    if (op == "deposito") {
        if(method == "PUT") {
            string body;
            cin >> body;
            vector<int> values = parseOperation(body);
            if(values.size() == 2){
                result = handleDepositar(values[0], values[1]);
                error = 0;
            }
        }
    }

    if (op == "extraccion") {
        if(method == "PUT") {
            string body;
            cin >> body;
            vector<int> values = parseOperation(body);
            if(values.size() == 2){
                result = handleRetirar(values[0], values[1]);
                error = 0;
            }
        }
    }


    if(error) 
        printf("Status: 400 Bad Request\n\n") ;
    else printf("Status: 200 OK\n\n");
    if(result.length() > 0) {
        printf("%s\n", result.c_str());    
    }
    
    return 0;
}