# Etapa 2

En esta etapa es posible conectar múltiples clientes y consumidores en una misma computadora.

## Build de la solución

La construcción de este sistema se hace mediante le uso de cmake, para ello es necesario ejecutar las siguientes sentencias ubicado en el directorio raíz:

```{r, engine='bash'}
$ mkdir build
$ cd build
$ cmake ..
$ make
```
Una vez hecho esto ya se encuentran disponibles los ejecutables de la solución.

## Inicializar los recursos compartidos 

Antes de poder ejecutar los traductores y clientes es necesario iniciar los recursos compartidos.
```{r, engine='bash'}
$ ./Middleware 
```


## Ejecutando Productores y Traductores
Tanto Productores como Traductores pueden ser ejecutados desde linea de comando de la siguiente manera


```{r, engine='bash'}
//Ejecutar un cliente
$ ./Cliente

//Ejecutar un traductor
$ ./Traductor & 
```

## Liberar recursos compartidos

```{r, engine='bash'}
$ ./Destructor
```