/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _CAJERO_H_RPCGEN
#define _CAJERO_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MOVIMIENTOSC 10

struct cuenta_args {
	int cuenta;
	int cantidad;
};
typedef struct cuenta_args cuenta_args;

struct movimiento_t {
	int tipo;
	int cantidad;
};
typedef struct movimiento_t movimiento_t;

typedef struct {
	u_int movimientos_t_len;
	movimiento_t *movimientos_t_val;
} movimientos_t;

struct historico_t {
	int result;
	movimientos_t movs;
};
typedef struct historico_t historico_t;

#define CAJEROPROG 0x20000002
#define CAJEROVERSION 1

#if defined(__STDC__) || defined(__cplusplus)
#define CONSULTASALDO 1
extern  int * consultasaldo_1(int *, CLIENT *);
extern  int * consultasaldo_1_svc(int *, struct svc_req *);
#define DEPOSITAR 2
extern  int * depositar_1(cuenta_args *, CLIENT *);
extern  int * depositar_1_svc(cuenta_args *, struct svc_req *);
#define RETIRAR 3
extern  int * retirar_1(cuenta_args *, CLIENT *);
extern  int * retirar_1_svc(cuenta_args *, struct svc_req *);
#define MOVIMIENTOS 4
extern  historico_t * movimientos_1(int *, CLIENT *);
extern  historico_t * movimientos_1_svc(int *, struct svc_req *);
#define CREAR 5
extern  int * crear_1(int *, CLIENT *);
extern  int * crear_1_svc(int *, struct svc_req *);
extern int cajeroprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define CONSULTASALDO 1
extern  int * consultasaldo_1();
extern  int * consultasaldo_1_svc();
#define DEPOSITAR 2
extern  int * depositar_1();
extern  int * depositar_1_svc();
#define RETIRAR 3
extern  int * retirar_1();
extern  int * retirar_1_svc();
#define MOVIMIENTOS 4
extern  historico_t * movimientos_1();
extern  historico_t * movimientos_1_svc();
#define CREAR 5
extern  int * crear_1();
extern  int * crear_1_svc();
extern int cajeroprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_cuenta_args (XDR *, cuenta_args*);
extern  bool_t xdr_movimiento_t (XDR *, movimiento_t*);
extern  bool_t xdr_movimientos_t (XDR *, movimientos_t*);
extern  bool_t xdr_historico_t (XDR *, historico_t*);

#else /* K&R C */
extern bool_t xdr_cuenta_args ();
extern bool_t xdr_movimiento_t ();
extern bool_t xdr_movimientos_t ();
extern bool_t xdr_historico_t ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_CAJERO_H_RPCGEN */