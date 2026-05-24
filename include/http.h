#ifndef HTTP_H
#define HTTP_H

/* Procesa una request HTTP recibida desde un cliente */
void handle_http_request(
    int client_fd,
    const char *request);

#endif