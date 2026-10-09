// Proporciona printf, fprintf y perror para mostrar informacion y errores.
#include <stdio.h>
// Proporciona funciones generales de la biblioteca estandar.
#include <stdlib.h>
// Proporciona las constantes necesarias para open, como O_CREAT y O_RDWR.
#include <fcntl.h>
// Proporciona getopt, optarg y las llamadas POSIX al sistema.
#include <unistd.h>
// Proporciona la declaracion de getopt y sus variables auxiliares.
#include <getopt.h>

// argc contiene el numero de argumentos y argv contiene sus textos.
int main(int argc, char *argv[]) {

    // Guarda la opcion que getopt encuentra en cada vuelta del bucle.
    int opt;
    // Estas variables indican si se pidio lectura, escritura y un nombre de fichero.
    int readable = 0, writable = 0, hasFile = 0;
    // Guarda el nombre del fichero indicado despues de la opcion -f.
    const char *fileName = NULL;

    // Analiza las opciones -r, -w y -f; los dos puntos indican que -f necesita un argumento.
    while ((opt = getopt(argc, argv, "rwf:")) != -1) {
        // Decide que hacer segun la opcion encontrada.
        switch (opt) {
            case 'r':
                // Indica que el fichero se abrira con permiso de lectura.
                readable = 1;
                break;
            case 'w':
                // Indica que el fichero se abrira con permiso de escritura.
                writable = 1;
                break;
            case 'f':
                // optarg contiene el nombre escrito despues de -f.
                fileName = optarg;
                // Marca que se ha proporcionado el nombre del fichero.
                hasFile = 1;
                break;
            default:
                // Termina si se recibe una opcion desconocida o incorrecta.
                return 1;
        }
    }

    // Comprueba que exista -f y que se haya pedido lectura, escritura o ambas.
    if (!hasFile || (!readable && !writable)) {
        // Muestra la forma correcta de ejecutar el programa por la salida de errores.
        fprintf(stderr, "Uso: %s [-r] [-w] -f fichero\n", argv[0]);
        return 1;
    }

    // O_CREAT crea el fichero si no existe y O_TRUNC borra su contenido si ya existe.
    int flags = O_CREAT | O_TRUNC;

    // Selecciona el modo de apertura segun las opciones recibidas.
    if (readable && writable)
        // O_RDWR permite leer y escribir.
        flags |= O_RDWR;
    else if (readable)
        // O_RDONLY permite solamente leer.
        flags |= O_RDONLY;
    else
        // O_WRONLY permite solamente escribir.
        flags |= O_WRONLY;

    // Abre o crea el fichero con los flags seleccionados.
    // 0666 establece los permisos iniciales si es necesario crear el fichero.
    int fd = open(fileName, flags, 0666);
    // open devuelve -1 cuando no puede abrir o crear el fichero.
    if (fd == -1) {
        // perror muestra el motivo del error proporcionado por el sistema.
        perror("open");
        return 1;
    }

    // Muestra el descriptor que el sistema ha asignado al fichero abierto.
    printf("Fichero abierto correctamente (descriptor: %d)\n", fd);

    // Texto que el programa intentara escribir en el fichero.
    const char text[] = "Perico de los palotes tiene algo que decir\n";
    // Buffer donde se guardara el contenido leido del fichero.
    char buffer[sizeof(text)];
    // Guarda la cantidad de bytes procesados por write o read.
    ssize_t bytes;

    // Intenta escribir el texto independientemente del modo de apertura elegido.
    bytes = write(fd, text, sizeof(text) - 1);
    if (bytes == -1) {
        // perror muestra el motivo por el que write no ha podido escribir.
        perror("write");
    } else {
        // Informa de cuantos bytes ha escrito write.
        printf("Bytes escritos: %zd\n", bytes);
    }

    // Vuelve al principio para que read pueda leer el contenido del fichero.
    if (lseek(fd, 0, SEEK_SET) == -1) {
        // perror muestra el motivo si no se puede cambiar la posicion.
        perror("lseek");
    }

    // Intenta leer el contenido independientemente del resultado de write.
    bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes == -1) {
        // perror muestra el motivo por el que read no ha podido leer.
        perror("read");
    } else {
        // Anade el terminador para poder mostrar buffer como una cadena.
        buffer[bytes] = '\0';
        printf("Bytes leidos: %zd\nContenido: %s", bytes, buffer);
    }

    // Termina correctamente el programa.
    return 0;
}

