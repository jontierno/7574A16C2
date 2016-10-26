## Motivación

Ejemplo simple de "servidor echo" iterativo. Cada cliente es atendido secuencialmente.

##Compilación

```{r, engine='bash'}
make server
make client
```
##Ejecución Servidor

```{r, engine='bash'}
./server & (esto corre el servidor en background).
```

## Ejecución clientes
Se pueden ejecutar cuantos clientes se quiera pero quedarán esperando hasta que el servidor termine la tarea en curso y luego tomará el cliente siguiente.

```{r, engine='bash'}
sleep(rand()%3);
```

