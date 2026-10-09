#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // Define los flags O_RDONLY, O_WRONLY, O_CREAT...
#include <unistd.h> // Funciones estándar POSIX como read, write, close...
#include <errno.h> // Variable global errno para chequear causas de errores

// Función que clona de fdo (descriptor origen) a fdd (descriptor destino)
int copy(int fdo, int fdd)
{
    char buffer[512]; // Array en la pila para el chunk temporal
    ssize_t bytes_read; // ssize_t permite números negativos para indicar errores, size_t no.
    ssize_t bytes_written;

    // read retorna bytes leídos. Si retorna > 0, procesamos el buffer. 0 es EOF. -1 es error.
    while ((bytes_read = read(fdo, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(fdd, buffer, bytes_read); // Escribe exactamente lo que se leyó

        if (bytes_written == -1) {
            return -1; // Fallo de escritura
        }

        // Caso raro: si no escribió TODO el chunk, consideramos que falló (ej: disco lleno)
        if (bytes_written != bytes_read) {
            errno = EIO; // Seteamos el error I/O manualmente
            return -1;
        }
    }

    // Si salimos del while porque bytes_read == -1, devolvemos error
    return bytes_read == -1 ? -1 : 0; 
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }

	// open es la syscall POSIX base, devuelve un int (file descriptor), no un puntero a FILE
	int fdo = open(argv[1], O_RDONLY); // Solo lectura
	if (fdo == -1) {
		perror("The source file could not be opened");
		return 2;
	}

	// O_CREAT requiere el 3er parámetro (permisos octales 0666 = lectura/escritura).
	// O_TRUNC vacía el archivo a 0 bytes si ya existía antes de abrirlo.
	int fdd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fdd == -1) {
		perror("The destination file could not be opened");
		close(fdo); // Siempre cerrar origen si fallamos acá para evitar leaks de descriptores
		return 3;
	}

	if (copy(fdo, fdd) == -1) {
		perror("The file could not be copied");
		close(fdo);
		close(fdd);
		return 4;
	}

	// Siempre cerrar los File Descriptors antes de terminar
	close(fdo);
	close(fdd);

	return 0;
}
