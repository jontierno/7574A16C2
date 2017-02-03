# CAJERO RMI

En esta implementación se utiliza la tecnología RMI y lenguaje Java para que los clientes tengan acceso remoto al servidor que maneja las operaciones sobre las cuentas.

## Build de la solución

La construcción de este sistema se hace mediante le uso de gradle, para ello es necesario ejecutar las siguientes sentencias ubicado en el directorio raíz:

```{r, engine='bash'}
./gradlew build
```
Una vez hecho esto ya se encuentran disponibles los ejecutables de la solución en el directorio *build*.


## Ejecutando el servidor en segundo plano

```{r, engine='bash'}
java -jar build/CajeroServer.jar &
```

## Ejecutando el cliente

```{r, engine='bash'}
java -jar build/CajeroClient.jar
```
La aplicación desplegará un menú con las operaciones disponibles.


## Generar projectos Eclipse (opcional)

```{r, engine='bash'}
./gradlew eclipse 
```
Una vez hecho esto es posible importar los proyectos dentro de eclipse.
