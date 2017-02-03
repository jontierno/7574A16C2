
##Como correr los ejercicios




### Ejercicio "exclusionmutua" ejemplifica Dos procesos excluyendose utilizando exclusión mutua.

```{r, engine='bash'}
$ make exclusionmutua
$ ./exclusionmutua
```


### Ejercicio "barrera" ejemplifica procesos coordinados con una barrera con semáforos generales.

```{r, engine='bash'}
$ make barrera
$ ./barrera
 ```

###Ejercicio "barrera_nuevo" ejemplifica procesos coordinados con una barrera pero con semáforos unitarios.

```{r, engine='bash'}
$ make barrera_nuevo
$ ./barrera_nuevo
```
###Ejercicio "multiplesprodcons" Es un problema de productor-consumidor donde hay N productores que generan K numeros (en total K*N numeros generados) y los M consumidores deben leer todos el número antes de que se pueda generar uno nuevo (en total cada consumidor espera leer K*N números)

```{r, engine='bash'}
$ make multiplesprodcons
$ ./multiplesprodcons
```

### Ejercicio "multiplesprocsconsmult". Problema clásico de productor-consumidor con buffer de tamaño fijo circular

```{r, engine='bash'}
$ make multiplesprocsconsmult
$ ./multiplesprocsconsmult
```

##Liberar recursos
Para liberar los recursos IPC después de cualquiera de los ejemplos es necesario ejecutar.

```{r, engine='bash'}
$ make destructor 
$ ./destructor
```