#include "http.h"
#include "files.h"

#include <stdio.h>
#include <string.h>

#include <sys/socket.h>

#define METHOD_SIZE 16
#define URI_SIZE 1024
#define VERSION_SIZE 16

/*
    Procesa request HTTP.
*/
int handle_http_request(
    int client_fd,
    const char *request)
{
    char method[METHOD_SIZE];

    char uri[URI_SIZE];

    char version[VERSION_SIZE];

    /*
        Headers básicos HTTP/1.1.
    */
    char host[256] = "";

    char connection[256] = "";

    char user_agent[512] = "";

    /*
        Extrae:
        método, URI y versión.
    */
    int parsed = sscanf(
        request,
        "%15s %1023s %15s",
        method,
        uri,
        version);

    /*
        Request inválida.
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

        return 0;
    }

    /*
        Parsing headers básicos.
    */
    char *host_ptr =
        strstr(request, "Host:");

    if (host_ptr != NULL)
    {
        sscanf(
            host_ptr,
            "Host: %255[^\r\n]",
            host);
    }

    char *connection_ptr =
        strstr(request, "Connection:");

    if (connection_ptr != NULL)
    {
        sscanf(
            connection_ptr,
            "Connection: %255[^\r\n]",
            connection);
    }

    char *user_agent_ptr =
        strstr(request, "User-Agent:");

    if (user_agent_ptr != NULL)
    {
        sscanf(
            user_agent_ptr,
            "User-Agent: %511[^\r\n]",
            user_agent);
    }

    printf("\n=== PARSED REQUEST ===\n");

    printf("Method: %s\n", method);

    printf("URI: %s\n", uri);

    printf("Version: %s\n", version);

    printf("Host: %s\n", host);

    printf("Connection: %s\n", connection);

    printf("User-Agent: %s\n", user_agent);

    /*
        Solo GET permitido.
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

        return 0;
    }

    /*
        Validamos versión.
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

        return 0;
    }

    /*
        Ruta principal.
    */
    if (strcmp(uri, "/") == 0)
    {
        send_file(
            client_fd,
            "www/index.html");

        /*
            Keep-alive simple.
        */
        if (
            strcmp(
                connection,
                "keep-alive") == 0)
        {
            return 1;
        }

        return 0;
    }

    /*
        Construimos ruta.
    */
    char path[2048];

    snprintf(
        path,
        sizeof(path),
        "www%s",
        uri);

    send_file(
        client_fd,
        path);

    /*
        Keep-alive simple.
    */
    if (
        strcmp(
            connection,
            "keep-alive") == 0)
    {
        return 1;
    }

    return 0;
}