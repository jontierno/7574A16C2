###Productor Consumidor con conlas de mensajes.

En esta implementación los productores y consumidores se comunican utilizando colas de mensajes nativas de Unix

## Build de la solución

make consumidor && make productor && make lanzador && make destructor

## Correr la solución


```{r, engine='bash'}
$ ./lanzador
```

## Parar la solución


```{r, engine='bash'}
$ ./destructor
```

##Limpieza de recursos
Si por algún motivo quedaran recursos sin liberar, se debe ejecutar


```{r, engine='bash'}
$ ./destructor
```