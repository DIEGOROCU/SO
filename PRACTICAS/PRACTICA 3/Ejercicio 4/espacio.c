#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // Para strcmp, strlen
#include <sys/stat.h> // Syscall lstat, struct stat y macro S_ISDIR
#include <dirent.h>   // Funciones de directorio: opendir, readdir, closedir
#include <limits.h>   // Constante PATH_MAX para el tamaño del buffer de rutas

/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks)
{
	struct stat st;

	// lstat consulta los metadatos del i-nodo sin seguir enlaces simbólicos (evita bucles)
	if (lstat(fname, &st) == -1) {
		perror(fname);
		return -1;
	}

	// st_blocks contiene el número de bloques de 512 bytes reservados en disco por el SO
	*blocks += (size_t)st.st_blocks;

	// Macro S_ISDIR comprueba los bits de tipo en st_mode para identificar si es directorio
	if (S_ISDIR(st.st_mode)) {
		return get_size_dir(fname, blocks);
	}

	return 0;
}

/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks)
{
	// opendir abre un descriptor de flujo de directorio DIR*
	DIR *dir = opendir(dname);
	if (dir == NULL) {
		perror(dname);
		return -1;
	}

	struct dirent *entry;
	char path[PATH_MAX];

	// readdir itera por cada entrada del directorio de manera no ordenada
	while ((entry = readdir(dir)) != NULL) {
		// Ignorar . y .. para prevenir bucles de recursión infinita
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		// Construir la ruta relativa completa dname/entry->d_name
		if (dname[strlen(dname) - 1] == '/') {
			snprintf(path, sizeof(path), "%s%s", dname, entry->d_name);
		} else {
			snprintf(path, sizeof(path), "%s/%s", dname, entry->d_name);
		}

		// Llamar a get_size para acumular bloques del hijo y descender si es subdirectorio
		get_size(path, blocks);
	}

	// Cerrar siempre el flujo de directorio para liberar descriptores y memoria asociada
	closedir(dir);
	return 0;
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
	// Verificar que se haya pasado al menos un fichero o directorio por argumento
	if (argc < 2) {
		fprintf(stderr, "Uso: %s <fichero1> [fichero2 ...]\n", argv[0]);
		return 1;
	}

	// Procesar cada ruta recibida como parámetro en la línea de comandos
	for (int i = 1; i < argc; i++) {
		size_t blocks = 0; // Inicializar a cero para acumular los bloques del fichero actual
		if (get_size(argv[i], &blocks) == 0) {
			// Cada bloque de st_blocks es de 512B; sumamos 1 y dividimos por 2 para redondear a KiB (1024B)
			size_t kb = (blocks + 1) / 2;
			printf("%zuK %s\n", kb, argv[i]);
		}
	}

	return 0;
}
