#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    // Abrir output.txt para escribir, crearlo si no existe y vaciarlo antes.
    // 0600 permite leer y escribir al propietario, pero no a otros usuarios.
    int fd = open("output.txt", O_CREAT | O_TRUNC | O_WRONLY, 0600);
    int i;

    // open devuelve -1 si no pudo abrir o crear el fichero.
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    // pwrite recibe descriptor, datos, cantidad y posición absoluta del fichero.
    // El padre escribe el bloque inicial de ceros en las posiciones 0 a 4.
    if (pwrite(fd, "00000", 5, 0) == -1) {
        perror("pwrite");
        close(fd);
        return EXIT_FAILURE;
    }

    // Crear nueve hijos; cada uno escribirá un bloque distinto.
    for (i = 1; i < 10; i++) {
        pid_t pid = fork();

        // fork devuelve -1 si falla, 0 al hijo y el PID del hijo al padre.
        if (pid == -1) {
            perror("fork");
            close(fd);
            return EXIT_FAILURE;
        }

        if (pid == 0) {
            char block[5];
            int j;

            // Convertir el número del hijo en cinco caracteres iguales.
            for (j = 0; j < 5; j++) {
                block[j] = '0' + i;
            }

            // i * 5 coloca el bloque justo después del bloque anterior.
            // pwrite usa una posición propia y evita carreras con lseek/write.
            if (pwrite(fd, block, sizeof(block),  i * 5) == -1) {
                perror("pwrite");
                _exit(EXIT_FAILURE);
            }
            close(fd);
            _exit(EXIT_SUCCESS);
        }
    }

    // wait devuelve el PID de un hijo terminado y -1 cuando ya no quedan.
    while (wait(NULL) > 0) {
    }

    close(fd);
    return EXIT_SUCCESS;
}
