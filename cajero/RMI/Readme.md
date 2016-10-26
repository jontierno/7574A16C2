# CAJERO RMI

El siguiente repositorio contiene implementaciones de un cajero via RMI.

## Build de la solución

La construcción de este sistema se hace mediante le uso de gradle, para ello es necesario ejecutar las siguientes sentencias parado en el directorio raíz:

```{r, engine='bash'}
./gradlew build
```
Una vez hecho esto ya se encuentran disponibles los ejecutables de la solución en el directorio *build*.

## Generar projectos Eclipse

```{r, engine='bash'}
./gradlew eclipse 
```
Una vez hecho esto es posible importar los proyectos dentro de eclipse.

## Ejecutando el servidor en segundo plano

```{r, engine='bash'}
java -jar build/CajeroServer.jar &
```

## Ejecutando el cliente

```{r, engine='bash'}
java -jar build/CajeroClient.jar
```
La aplicación desplegará un menú con las operaciones disponibles.