#ifndef HTTP_H
#define HTTP_H

/*
    Procesa request HTTP.

    Retorna:
    1 = mantener conexión
    0 = cerrar conexión
*/
int handle_http_request(
    int client_fd,
    const char *request);

#endif