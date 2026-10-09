#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h> // Funciones lstat/stat y macros S_ISREG, S_ISLNK...
#include <errno.h>

int copy(int fdo, int fdd)
{
	char buffer[512];
	ssize_t bytes_read;

	/* Leer y escribir el fichero regular en bloques de 512 bytes mediante syscalls POSIX. */
	while ((bytes_read = read(fdo, buffer, sizeof(buffer))) > 0) {
		ssize_t bytes_written = write(fdd, buffer, bytes_read);

		/* Una escritura incompleta (menos bytes de los leídos) no permite copiar correctamente. */
		if (bytes_written != bytes_read) {
			if (bytes_written != -1) {
				errno = EIO; // Forzar error de Entrada/Salida
			}
			return -1;
		}
	}

	/* read devuelve cero al llegar al final (EOF) o -1 si ocurre un error. */
	return bytes_read == -1 ? -1 : 0;
}

int copy_regular(const char *orig, const char *dest)
{
	/* Abrir origen modo lectura. (System call open, más rápida/baja que fopen) */
	int fdo = open(orig, O_RDONLY);
	if (fdo == -1) {
		return -1;
	}

    // O_TRUNC resetea el archivo a 0 bytes. O_CREAT exige permisos (0666) en octal
	int fdd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fdd == -1) {
		close(fdo); // Libera fd si falla la apertura del 2do
		return -1;
	}

	/* Copiar el contenido y cerrar ambos descriptores usando close(). */
	int result = copy(fdo, fdd);
	close(fdo);
	close(fdd);

	return result;
}

int copy_link(const char *orig, const char *dest, off_t link_size)
{
	/* Reservar espacio en Heap para la ruta que apunta el link. sumando +1 para el '\0'. */
	char *target = malloc((size_t)link_size + 1);
	if (target == NULL) {
		return -1;
	}

	/* readlink lee a qué ruta apunta el link. CUIDADO: No inserta el terminador '\0'. */
	ssize_t target_length = readlink(orig, target, (size_t)link_size + 1);
	if (target_length == -1) {
		free(target);
		return -1;
	}

    // Añadir el caracter nulo manualmente para poder tratar target como una string de C válida
	target[target_length] = '\0';

	/* symlink crea un nuevo link apuntando a 'target' llamado 'dest'. */
	int result = symlink(target, dest);
	free(target); // Liberar SIEMPRE memoria malloc tras uso

	return result;
}

int main(int argc, char *argv[])
{
	struct stat file_info; // struct proporcionada por sys/stat.h con info detallada (inode)

	/* Comprobar que se han recibido origen y destino. */
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
		return 1;
	}

	/* lstat examina el enlace en sí, sin seguirlo hasta su destino (stat normal sí lo sigue). */
	if (lstat(argv[1], &file_info) == -1) {
		perror("The source file could not be inspected");
		return 2;
	}

	/* Elegir la copia según el tipo real extraído del flag st_mode devuelto por lstat. */
	if (S_ISREG(file_info.st_mode)) { // Macro S_ISREG: Es archivo regular?
		if (copy_regular(argv[1], argv[2]) == -1) {
			perror("The regular file could not be copied");
			return 3;
		}
	} else if (S_ISLNK(file_info.st_mode)) { // Macro S_ISLNK: Es link simbólico?
		if (copy_link(argv[1], argv[2], file_info.st_size) == -1) {
			perror("The symbolic link could not be copied");
			return 4;
		}
	} else {
		fprintf(stderr, "The source file is neither regular nor symbolic link\n");
		return 5;
	}

	return 0;
}
