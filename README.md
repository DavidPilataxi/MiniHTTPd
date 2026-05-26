# Proyecto MiniHTTPd - Computación Distribuida

> Servidor HTTP/1.1 básico desarrollado en C utilizando sockets TCP y `epoll`, sin bibliotecas HTTP externas.

---

## Descripción del proyecto

Este proyecto implementa un servidor HTTP/1.1 básico para Linux desarrollado completamente en C. El servidor es capaz de atender múltiples clientes concurrentes, servir archivos estáticos y procesar solicitudes HTTP mediante sockets TCP.

El objetivo del proyecto es comprender el funcionamiento interno de un servidor web, incluyendo parsing HTTP, manejo de conexiones, acceso al sistema de archivos y seguridad básica.

---

## Características

* Método `GET`
* Archivos estáticos (`html`, `css`, `js`, imágenes)
* Múltiples clientes con `epoll`
* Conexiones persistentes (`keep-alive`)
* Tipos MIME
* Códigos HTTP
* Validación de solicitudes
* Protección contra vulnerabilidades comunes

---

## Tecnologías utilizadas

* Lenguaje C
* Linux / Ubuntu
* WSL Ubuntu
* Sockets TCP
* epoll
* GCC
* Makefile

---

## Estructura del proyecto

```text id="t4r2ps"
## Estructura del proyecto

```text
minihttpd/
├── Makefile         Compilación automática del proyecto
├── README.md        Documentación del proyecto
├── include/         Archivos de cabecera (.h)
├── src/             Código fuente principal del servidor
├── www/             Archivos estáticos servidos por el servidor
```


---

## Requisitos

* Sistema Linux o WSL Ubuntu
* GCC
* Make
* Terminal Bash

Instalar dependencias:

```bash id="n5ch1r"
sudo apt update
sudo apt install build-essential
```

---

## Instalación y uso

### Opción 1: Clonar el repositorio

```bash id="6wjlwm"
git clone https://github.com/DavidPilataxi/MiniHTTPd.git
cd MiniHTTTPd
```

### Opción 2: Descargar el proyecto

* Descargar el repositorio.
* Mover la carpeta del proyecto dentro de un directorio con entorno Linux o WSL Ubuntu. 
* Abrir la terminal en el directorio del proyecto.

---

## Compilación

```bash id="g5o6mx"
make
```

---

## Ejecución

```bash id="8hrzjj"
./minihttpd
```

Abrir en navegador:

```text id="e2u6r7"
http://localhost:8080
```

---

## Pruebas

```bash id="fp0m5y"
curl http://localhost:8080
```

Mostrar encabezados HTTP:

```bash id="l0o3lj"
curl -i http://localhost:8080
```

Solicitar archivos:

```bash id="49qft0"
curl http://localhost:8080/style.css
curl http://localhost:8080/test.js
```

---

## Tipos MIME soportados

```c id="uy1z2k"
{".html", "text/html"},
{".css",  "text/css"},
{".js",   "application/javascript"},
{".png",  "image/png"},
{".jpg",  "image/jpeg"}
```

---

## Códigos HTTP implementados

* `200 OK`
* `400 Bad Request`
* `403 Forbidden`
* `404 Not Found`
* `405 Method Not Allowed`
* `500 Internal Server Error`

---

## Seguridad

El servidor implementa protección contra:

* Directory Traversal
* Buffer Overflows
* Métodos HTTP inválidos
* Solicitudes malformadas

---

## Archivos Zone.Identifier

Al mover la carpeta desde Windows a un entorno linux como WSL pueden aparecer archivos como:

```text id="e8k8nn"
archivo.c:Zone.Identifier
```

Estos archivos son metadatos generados por Windows al descargar archivos desde internet.

No afectan:

* la compilación
* la ejecución
* `gcc`
* `make`
* el servidor HTTP

Para eliminarlos:

```bash id="59fg7l"
find . -name "*:Zone.Identifier" -type f -delete
```

---

## Autor

David Pilataxi — Computación Distribuida
