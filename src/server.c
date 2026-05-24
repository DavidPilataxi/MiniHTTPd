#define _POSIX_C_SOURCE 200112L
#include "server.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
    Configura un socket como
    no bloqueante.
*/
static int set_nonblocking(
    int fd)
{
    /*
        Obtiene flags actuales.
    */
    int flags = fcntl(
        fd,
        F_GETFL,
        0);

    if (flags == -1)
    {
        return -1;
    }

    /*
        Agrega flag O_NONBLOCK.
    */
    return fcntl(
        fd,
        F_SETFL,
        flags | O_NONBLOCK);
}

/* Crea socket TCP servidor */
int create_server_socket(
    const char *port)
{
    struct addrinfo hints;

    struct addrinfo *result;

    struct addrinfo *rp;

    int server_fd;

    int yes = 1;

    memset(
        &hints,
        0,
        sizeof(hints));

    hints.ai_family = AF_UNSPEC;

    hints.ai_socktype = SOCK_STREAM;

    hints.ai_flags = AI_PASSIVE;

    if (
        getaddrinfo(
            NULL,
            port,
            &hints,
            &result) != 0)
    {
        perror("getaddrinfo");

        return -1;
    }

    for (
        rp = result;
        rp != NULL;
        rp = rp->ai_next)
    {
        server_fd = socket(
            rp->ai_family,
            rp->ai_socktype,
            rp->ai_protocol);

        if (server_fd == -1)
        {
            continue;
        }

        setsockopt(
            server_fd,
            SOL_SOCKET,
            SO_REUSEADDR,
            &yes,
            sizeof(yes));

        if (
            bind(
                server_fd,
                rp->ai_addr,
                rp->ai_addrlen) == 0)
        {
            break;
        }

        close(server_fd);
    }

    freeaddrinfo(result);

    if (rp == NULL)
    {
        perror("Could not bind");

        return -1;
    }

    /*
        Socket no bloqueante.
    */
    if (
        set_nonblocking(server_fd) == -1)
    {
        perror("fcntl");

        close(server_fd);

        return -1;
    }

    if (
        listen(
            server_fd,
            128) == -1)
    {
        perror("listen");

        close(server_fd);

        return -1;
    }

    return server_fd;
}