#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Funciones POSIX como getopt, read, write, close, lseek
#include <fcntl.h>  // Definición de flags de apertura como O_RDONLY
#include <errno.h>  // Variable global errno para reporte de errores
#include <getopt.h>


int main(int argc, char *argv[])
{
	int opt;
	long n_bytes = 0; // Valor por defecto de N si no se indica el flag -n
	int flag_e = 0;   // 1 si se indica -e (leer desde el final), 0 si se salta desde el inicio

	// getopt analiza las opciones de la línea de comandos; "n:e" indica que -n requiere argumento y -e no
	while ((opt = getopt(argc, argv, "n:e")) != -1) {
		switch (opt) {
		case 'n':
			// optarg contiene la cadena asociada al argumento de la opción actual
			n_bytes = atol(optarg);
			if (n_bytes < 0) {
				n_bytes = 0;
			}
			break;
		case 'e':
			flag_e = 1;
			break;
		default:
			fprintf(stderr, "Uso: %s [-n N] [-e] <archivo>\n", argv[0]);
			return 1;
		}
	}

	// optind apunta al primer argumento posicional tras haber procesado todas las opciones
	if (optind >= argc) {
		fprintf(stderr, "Error: falta indicar el nombre del archivo\n");
		fprintf(stderr, "Uso: %s [-n N] [-e] <archivo>\n", argv[0]);
		return 1;
	}

	// open devuelve un descriptor de archivo entero (fd); O_RDONLY abre en modo de solo lectura
	int fd = open(argv[optind], O_RDONLY);
	if (fd == -1) {
		perror("Error al abrir el archivo");
		return 2;
	}

	// lseek posiciona el offset del fichero; SEEK_END cuenta desde el final y SEEK_SET desde el inicio
	if (flag_e) {
		// Con -e situamos el marcador N bytes antes del final del fichero
		if (lseek(fd, -((off_t)n_bytes), SEEK_END) == (off_t)-1) {
			// Si N es mayor que el tamaño del archivo, lseek falla con EINVAL y nos colocamos al principio
			lseek(fd, 0, SEEK_SET);
		}
	} else {
		// Sin -e avanzamos el marcador N bytes desde el principio del fichero
		if (lseek(fd, (off_t)n_bytes, SEEK_SET) == (off_t)-1) {
			perror("Error al posicionar el marcador con lseek");
			close(fd);
			return 3;
		}
	}

	char byte;
	ssize_t bytes_read;

	// Lectura byte a byte hasta alcanzar EOF (read devuelve 0) o error (read devuelve -1)
	while ((bytes_read = read(fd, &byte, 1)) > 0) {
		// Descriptor 1 es la salida estándar (STDOUT_FILENO)
		if (write(1, &byte, 1) != 1) {
			perror("Error al escribir en la salida estándar");
			close(fd);
			return 4;
		}
	}

	// Comprobar si el bucle terminó por error de lectura
	if (bytes_read == -1) {
		perror("Error al leer el archivo");
		close(fd);
		return 5;
	}

	// Liberar siempre el descriptor de archivo del proceso
	close(fd);
	return 0;
}
