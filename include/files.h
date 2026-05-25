#ifndef FILES_H
#define FILES_H

/*
    Envía un archivo al cliente.

    client_fd:
    - socket del cliente

    path:
    - ruta del archivo solicitado

    Retorna:
    - 0 si tuvo éxito
    - -1 si hubo error
*/
int send_file(int client_fd, const char *path);

#endif