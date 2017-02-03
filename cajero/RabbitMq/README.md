# CAJERO MQTT(RabbitMQ)

En esta implementación se utiliza la tecnología RabbitMQ y lenguaje Java para que los clientes tengan acceso remoto al servidor que maneja las operaciones sobre las cuentas.


## Comunicacón dos vías.

Debido a que RabbitMQ es una tecnología para la utilización de colas de mensajes, y por tal hace invocaciones asíncronas, es necesario establecer un canal que permita enviar información al consumidor y otro para recibir la respuesta en el productor.

Es así que al momento de iniciarse un productor, éste genera un [UUID](https://es.wikipedia.org/wiki/Identificador_%C3%BAnico_universal) y crea un Exchange volátil utilizando este UUID en el nombre para que no se repita.

Al envíar cualquier pedido al consumidor, envía su UUID en los datos y queda a la espera de la respuesta en su exchange. El consumidor al recibir la operación, realiza la operación requerida y responde en el Exchange del productor que generó la petición.


## Build de la solución

La construcción de este sistema se hace mediante le uso de gradle, para ello es necesario ejecutar las siguientes sentencias ubicado en el directorio raíz:

```{r, engine='bash'}
$ ./gradlew build
```
Una vez hecho esto ya se encuentran disponibles los ejecutables de la solución en el directorio *build*.


## Ejecutando el consumidor (Server)

```{r, engine='bash'}
$ java -jar CajeroConsumer/build/libs/CajeroConsumer.jar &
```

## Ejecutando un cliente

```{r, engine='bash'}
$ java -jar CajeroProducer/build/libs/CajeroProducer.jar 
```
La aplicación desplegará un menú con las operaciones disponibles.


## Generar projectos Eclipse (opcional)

```{r, engine='bash'}
$ ./gradlew eclipse 
```
Una vez hecho esto es posible importar los proyectos dentro de eclipse.
