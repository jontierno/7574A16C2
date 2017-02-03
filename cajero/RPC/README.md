# Cajero RPC

En esta implementación se utiliza la tecnología RPC para que los clientes tengan acceso remoto al servidor que maneja las operaciones sobre las cuentas.

## Build de la solución

La construcción de este sistema se hace mediante le uso de cmake, para ello es necesario ejecutar las siguientes sentencias parado en el directorio raíz:

```{r, engine='bash'}
$ mkdir build
$ cd build
$ cmake ..
$ make
```
Una vez hecho esto ya se encuentran disponibles los ejecutables de la solución.

## Ejecución de la solución

### Ejecutando servicio RPC
Es necesario que el servicio de RPC se encuentre instalado y corriendo

```{r, engine='bash'}
$ sudo service rpcbind start
```

### Ejecutando el sevidor

```{r, engine='bash'}
$ ./build/server 
```
El servidor se encuentra esperando por conexiones de clientes.

### Ejecutando un cliente

```{r, engine='bash'}
$ ./build/client 
```
La aplicación desplegará un menu con las operaciones disponibles.