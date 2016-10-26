## Motivación

Ejemplo simple de comunicación entre dos procesos donde el cliente envia al servidor una linea que lee de stdin y el servidor responde lo mismo que lee.

##Compilación

```{r, engine='bash'}
make server
make client
```
##Ejecución

```{r, engine='bash'}
./server & (esto corre el servidor en background).
./client
```
