#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
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
    // El padre escribe el primer bloque de ceros en las posiciones 0 a 4.
    if (pwrite(fd, "00000", 5, 0) == -1) {
        perror("pwrite");
        close(fd);
        return EXIT_FAILURE;
    }

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

            // El hijo prepara cinco copias de su número.
            for (j = 0; j < 5; j++) {
                block[j] = '0' + i;
            }

            // i * 10 - 5 deja cinco ceros entre el bloque anterior y este.
            if (pwrite(fd, block, sizeof(block), i * 10 - 5) == -1) {
                perror("pwrite");
                _exit(EXIT_FAILURE);
            }
            close(fd);
            _exit(EXIT_SUCCESS);
        }

        // El padre espera para garantizar el orden de escritura.
        if (wait(NULL) == -1) {
            perror("wait");
            close(fd);
            return EXIT_FAILURE;
        }
        // No escribir ceros después del último bloque de nueves.
        if (i < 9 && pwrite(fd, "00000", 5, i * 10) == -1) {
            perror("pwrite");
            close(fd);
            return EXIT_FAILURE;
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}
