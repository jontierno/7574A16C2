# Cajero CORBA

Esta implementación  comunica la aplicación cliente cajero_client.py con el objeto que provee el servicio en el archivo 'cajero_impl.py' utilizando como medio de comunicación CORBA.

## Inicio del ORB

En este caso se utilizo [OmniORB](http://www.omniorb-support.com/), una vez instalado es necesario correr el servicio:

```{r, engine='bash'}
$ sudo service omniorb4-nameserver start
```

## Corer la solución


Primero es necesario correr el servicio que implementa las operaciones:

```{r, engine='bash'}
$ python cajero_impl.py
```

Y en paralelo se debe correr el cliente, el cual desplegará un menú con las operaciones disponibles

```{r, engine='bash'}
$ python cajero_client.py
```