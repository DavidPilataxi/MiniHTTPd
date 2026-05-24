#include "mime.h"

#include <string.h>

/* Recibe ruta archivo y devuelve Content-Type correspondiente */
const char *get_mime_type(
    const char *path)
{
    /*
        Busca la última aparición de '.'
        para obtener extensión.
    */
    const char *ext = strrchr(path, '.');

    /*
        Si no existe extensión,
        usamos binario genérico.
    */
    if (ext == NULL)
    {
        return "application/octet-stream";
    }

    /*
        HTML
    */
    if (strcmp(ext, ".html") == 0)
    {
        return "text/html";
    }

    /*
        CSS
    */
    if (strcmp(ext, ".css") == 0)
    {
        return "text/css";
    }

    /*
        JavaScript
    */
    if (strcmp(ext, ".js") == 0)
    {
        return "application/javascript";
    }

    /*
        PNG
    */
    if (strcmp(ext, ".png") == 0)
    {
        return "image/png";
    }

    /*
        JPG/JPEG
    */
    if (
        strcmp(ext, ".jpg") == 0 ||
        strcmp(ext, ".jpeg") == 0)
    {
        return "image/jpeg";
    }

    /*
        Tipo por defecto para
        archivos desconocidos.
    */
    return "application/octet-stream";
}