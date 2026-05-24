#include "http.h"
#include "files.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define METHOD_SIZE 16
#define URI_SIZE 1024
#define VERSION_SIZE 16

/* Procesa la request HTTP recibida */
void handle_http_request(
    int client_fd,
    const char *request)
{
    char method[METHOD_SIZE];

    char uri[URI_SIZE];

    char version[VERSION_SIZE];

    /*
        Extraemos:
        método, URI y versión.
    */
    int parsed = sscanf(
        request,
        "%15s %1023s %15s",
        method,
        uri,
        version);

    /*
        Request mal formada.
    */
    if (parsed != 3)
    {
        const char *response =
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 11\r\n"
            "\r\n"
            "Bad Request";

        send(
            client_fd,
            response,
            strlen(response),
            0);

        return;
    }

    printf("\n=== PARSED REQUEST ===\n");

    printf("Method: %s\n", method);

    printf("URI: %s\n", uri);

    printf("Version: %s\n", version);

    /*
        Solo permitimos método GET.
    */
    if (strcmp(method, "GET") != 0)
    {
        const char *response =
            "HTTP/1.1 405 Method Not Allowed\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 18\r\n"
            "\r\n"
            "Method Not Allowed";

        send(
            client_fd,
            response,
            strlen(response),
            0);

        return;
    }

    /*
        Validamos versión HTTP.
    */
    if (
        strcmp(version, "HTTP/1.1") != 0 &&
        strcmp(version, "HTTP/1.0") != 0)
    {
        const char *response =
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 11\r\n"
            "\r\n"
            "Bad Request";

        send(
            client_fd,
            response,
            strlen(response),
            0);

        return;
    }

    /*
        Ruta principal.
    */
    if (strcmp(uri, "/") == 0)
    {
        send_file(
            client_fd,
            "www/index.html");

        return;
    }

    /*
        Construimos ruta real:
        /style.css -> www/style.css
    */
    char path[2048];

    /*
        snprintf limita tamaño
        y evita overflow.
    */
    snprintf(
        path,
        sizeof(path),
        "www%s",
        uri);

    send_file(
        client_fd,
        path);
}