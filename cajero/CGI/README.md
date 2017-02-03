# Cajero CGI

En esta implementacion se provee una aplicación dividida en dos componentes principales, por un lado  una aplicación C++ para ser utilizada en un servidor CGI que expone una API para interactuar con ella y una pequeña aplicacion JavaScript utilizada como cliente de la API.

## API

### Crear una cuenta
* Método:  `POST`
* URL:  `/cuenta`
* FORM-DATA: `{id}`
* 
FORM-DATA de ejemplo
```sh
5
```

### Depositar en una cuenta
* Método:  `POST`
* URL:  `/deposito`
* FORM-DATA: `{id};{cantidad}`
* 
FORM-DATA de ejemplo
```sh
5;10
```

### Extraer de una cuenta
* Método:  `POST`
* URL:  `/extraccion`
* FORM-DATA: `{id};{cantidad}`
* 
FORM-DATA de ejemplo
```sh
5;20
```

### Obtener Saldo de una cuenta

* Método:  `GET`
* URL:  `/cuenta?{idcuenta}`

URL de ejemplo
```
/cuenta?5
```

### Obtener Movimientos de una cuenta

* Método:  `GET`
* URL:  `/movimientos?{idcuenta}`

URL de ejemplo
```
/movimientos?5
```

## Build de la solución

La construcción de este sistema se hace mediante le uso de cmake, para ello es necesario ejecutar las siguientes sentencias parado en el directorio raíz:

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```
Una vez hecho esto ya se encuentra disponible el ejecutable de la aplicación.

##Configuración de la aplicación CGI

Para poder utilizar la aplicación es necesario copiar el ejecutable al directorio del servidor CGI con el nombre cuenta y además crear una serie de vínculos simbólicos para que esten disponibles todas las urls de la api

```sh
$ cp cajero $CGI_ROOT/cuenta
$ ln -s $CGI_ROOT/cuenta $CGI_ROOT/movimientos
$ ln -s $CGI_ROOT/cuenta $CGI_ROOT/deposito
$ ln -s $CGI_ROOT/cuenta $CGI_ROOT/extraccion
```

##Configuración del cliente
Para poder tener disponbile el cliente es necesario copiar la carpeta cliente al directorio del servidor web.

```sh
$ cp client $ROOT_WEB/
```

##Ejecutar la aplicación

La aplicación se accede ingresando a la url provista por el servidor web, por ejemplo:

```
http://localhost
```


