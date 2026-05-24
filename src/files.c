#define _POSIX_C_SOURCE 200809L

#include "files.h"
#include "mime.h"

#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>

/*
    Declaración explícita
    de realpath().
*/
extern char *realpath(
    const char *path,
    char *resolved_path);

/* Envía archivo solicitado al cliente */
int send_file(
    int client_fd,
    const char *path)
{
    FILE *file;

    char *buffer;

    long file_size;

    char header[512];

    /*
        MIME type detectado
        según extensión.
    */
    const char *mime_type =
        get_mime_type(path);

    /*
        Rutas reales absolutas.

        Evita traversal real.
    */
    char real_requested[PATH_MAX];

    char real_www[PATH_MAX];

    /*
        Obtiene ruta absoluta
        del directorio www.
    */
    if (
        realpath(
            "www",
            real_www) == NULL)
    {
        const char *response =
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 21\r\n"
            "\r\n"
            "Internal Server Error";

        send(
            client_fd,
            response,
            strlen(response),
            0);

        return -1;
    }

    /*
        Detecta intento explícito
        de directory traversal.
    */
    if (strstr(path, "..") != NULL)
    {
        const char *response =
            "HTTP/1.1 403 Forbidden\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "403 Forbidden";

        send(
            client_fd,
            response,
            strlen(response),
            0);

        return -1;
    }

    /*
        Obtiene ruta absoluta
        solicitada.
    */
    if (
        realpath(
            path,
            real_requested) == NULL)
    {
        const char *response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "404 Not Found";

        send(
            client_fd,
            response,
            strlen(response),
            0);

        return -1;
    }

    /*
        Verifica que archivo
        esté dentro de www.
    */
    if (
        strncmp(
            real_requested,
            real_www,
            strlen(real_www)) != 0)
    {
        const char *response =
            "HTTP/1.1 403 Forbidden\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "403 Forbidden";

        send(
            client_fd,
            response,
            strlen(response),
            0);

        return -1;
    }

    /*
        Abrimos archivo binario.
    */
    file = fopen(path, "rb");

    /*
        Archivo inexistente.
    */
    if (file == NULL)
    {
        const char *response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "404 Not Found";

        send(
            client_fd,
            response,
            strlen(response),
            0);

        return -1;
    }

    /*
        Calculamos tamaño archivo.
    */
    fseek(file, 0, SEEK_END);

    file_size = ftell(file);

    rewind(file);

    /*
        Reserva memoria para
        guardar archivo completo.
    */
    buffer = malloc(file_size + 1);

    if (buffer == NULL)
    {
        fclose(file);

        const char *response =
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 21\r\n"
            "\r\n"
            "Internal Server Error";

        send(
            client_fd,
            response,
            strlen(response),
            0);

        return -1;
    }

    /*
        Leemos archivo completo.
    */
    fread(
        buffer,
        1,
        file_size,
        file);

    fclose(file);

    /*
        Construimos headers HTTP.
    */
    snprintf(
        header,
        sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "\r\n",
        mime_type,
        file_size);

    /*
        Enviamos headers HTTP.
    */
    send(
        client_fd,
        header,
        strlen(header),
        0);

    /*
        Enviamos contenido archivo.
    */
    send(
        client_fd,
        buffer,
        file_size,
        0);

    free(buffer);

    return 0;
}