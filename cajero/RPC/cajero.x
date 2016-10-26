const MOVIMIENTOSC = 10;	




struct cuenta_args {
int cuenta;
int cantidad;
};

struct movimiento_t {
	int tipo;
	int saldo;
};


typedef movimiento_t movimientos_t<MOVIMIENTOSC>;


struct historico_t {
	int result;
	movimientos_t movs;
};



program CAJEROPROG {
version CAJEROVERSION {
int CONSULTASALDO(int) = 1;
int DEPOSITAR(cuenta_args) = 2;
int RETIRAR(cuenta_args) = 3;
historico_t MOVIMIENTOS(int) = 4;
int CREAR(int) = 5;
} = 1;
} = 0x20000002;
