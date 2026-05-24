#ifndef SERVER_H
#define SERVER_H

/*
    Crea, configura y pone en escucha el socket del servidor.

    Recibe:
    - port: puerto donde escuchará el servidor

    Retorna:
    - descriptor del socket si todo sale bien
    - -1 si ocurre un error
*/
int create_server_socket(const char *port);

#endif