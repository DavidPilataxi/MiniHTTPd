#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>

#include <sys/socket.h>
#include <sys/epoll.h>

#include "server.h"
#include "http.h"

#define PORT "8080"

#define BUFFER_SIZE 4096

/*
    Máximo eventos simultáneos
    manejados por epoll.
*/
#define MAX_EVENTS 64

/* Punto entrada servidor */
int main(void)
{
    int server_fd;

    int epoll_fd;

    struct epoll_event event;

    struct epoll_event events[MAX_EVENTS];

    char buffer[BUFFER_SIZE];

    server_fd = create_server_socket(PORT);

    if (server_fd == -1)
    {
        return EXIT_FAILURE;
    }

    /*
        Creamos instancia epoll.
    */
    epoll_fd = epoll_create1(0);

    if (epoll_fd == -1)
    {
        perror("epoll_create1");

        close(server_fd);

        return EXIT_FAILURE;
    }

    /*
        Queremos monitorear:
        - lectura disponible
    */
    event.events = EPOLLIN;

    event.data.fd = server_fd;

    /*
        Registramos socket servidor.
    */
    if (
        epoll_ctl(
            epoll_fd,
            EPOLL_CTL_ADD,
            server_fd,
            &event) == -1)
    {
        perror("epoll_ctl");

        close(server_fd);

        close(epoll_fd);

        return EXIT_FAILURE;
    }

    printf(
        "Server listening on port %s\n",
        PORT);

    while (1)
    {
        /*
            Espera eventos sockets.
        */
        int event_count =
            epoll_wait(
                epoll_fd,
                events,
                MAX_EVENTS,
                -1);

        if (event_count == -1)
        {
            perror("epoll_wait");

            break;
        }

        /*
            Recorremos eventos listos.
        */
        for (
            int i = 0;
            i < event_count;
            i++)
        {
            /*
                Nuevo cliente.
            */
            if (
                events[i].data.fd == server_fd)
            {
                int client_fd =
                    accept(
                        server_fd,
                        NULL,
                        NULL);

                if (client_fd == -1)
                {
                    continue;
                }

                /*
                    Registramos cliente
                    en epoll.
                */
                event.events = EPOLLIN;

                event.data.fd =
                    client_fd;

                epoll_ctl(
                    epoll_fd,
                    EPOLL_CTL_ADD,
                    client_fd,
                    &event);
            }
            else
            {
                int client_fd =
                    events[i].data.fd;

                ssize_t bytes_received =
                    recv(
                        client_fd,
                        buffer,
                        BUFFER_SIZE - 1,
                        0);

                if (bytes_received <= 0)
                {
                    close(client_fd);

                    continue;
                }

                buffer[bytes_received] =
                    '\0';

                printf(
                    "\n=== RAW REQUEST ===\n");

                printf(
                    "%s\n",
                    buffer);

                /*
                    Procesa HTTP.
                */
                handle_http_request(
                    client_fd,
                    buffer);

                /*
                    Eliminamos cliente
                    de epoll.
                */
                epoll_ctl(
                    epoll_fd,
                    EPOLL_CTL_DEL,
                    client_fd,
                    NULL);

                close(client_fd);
            }
        }
    }

    close(server_fd);

    close(epoll_fd);

    return EXIT_SUCCESS;
}