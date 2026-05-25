#include "mime.h"

#include <string.h>

/* Detecta MIME type según extensión */
const char *get_mime_type(
    const char *path)
{
    const char *extension;

    extension = strrchr(path, '.');

    /*
        Archivo sin extensión.
    */
    if (extension == NULL)
    {
        return "text/plain";
    }

    if (strcmp(extension, ".html") == 0)
    {
        return "text/html";
    }

    if (strcmp(extension, ".css") == 0)
    {
        return "text/css";
    }

    if (strcmp(extension, ".js") == 0)
    {
        return "application/javascript";
    }

    if (strcmp(extension, ".png") == 0)
    {
        return "image/png";
    }

    if (strcmp(extension, ".jpg") == 0)
    {
        return "image/jpeg";
    }

    /*
        MIME default.
    */
    return "application/octet-stream";
}