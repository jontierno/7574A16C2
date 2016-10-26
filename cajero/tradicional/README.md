# Tradicional

En esta implementación no hay ningun tipo de separación entre el cliente de la aplicación y el cajero, es una única aplicación monolítica.


## Build de la solución

La construcción de este sistema se hace mediante le uso de cmake, para ello es necesario ejecutar las siguientes sentencias parado en el directorio raíz:

```{r, engine='bash'}
mkdir build
cd build
cmake ..
make
```
Una vez hecho esto ya se encuentran disponibles los ejecutables de la solución.

## Ejecución de la solución

```{r, engine='bash'}
./build/tradicional 
```
La aplicación desplegará un menu con las operaciones disponibles.