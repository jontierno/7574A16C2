# Etapa 1

En esta etapa tanto el cliente como el traductor corren en el mismo servidor unidos por colas de mensajes.

## Build de la solución

La construcción de este sistema se hace mediante le uso de cmake, para ello es necesario ejecutar las siguientes sentencias parado en el directorio raíz:

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


## Ejecutando Clientes y Traductores
Tanto Clientes como Traductores pueden ser ejecutados desde linea de comando de la siguiente manera


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