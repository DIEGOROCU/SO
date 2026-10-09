#include <stdio.h>
#include <stdlib.h>

char *loadstr(FILE *input)
{
    unsigned char c;
    size_t len = 0;
    long pos;
    char *str;
    
    // ftell() devuelve la posición actual del marcador dentro del archivo.
    pos = ftell(input);
    if (pos == -1L) {
        return NULL;
    }

    // Bucle infinito: lee byte a byte hasta encontrar '\0' para saber el tamaño real de la string.
    while (1) {
        if (fread(&c, sizeof(c), 1, input) != 1) {
            return NULL;
        }
        if (c == '\0') {
            break; // Salir al encontrar el delimitador de cadena
        }
        len++;
    }

    // fseek() rebobina el archivo al punto 'pos' guardado anteriormente (SEEK_SET: absoluto desde el inicio)
    if (fseek(input, pos, SEEK_SET) != 0) {
        return NULL;
    }

    // Malloc reserva memoria en el heap (requiere free en el main). +1 para el '\0'.
    str = malloc(len + 1);
    if (str == NULL) {
        return NULL; // Fallo de asignación de memoria
    }

    // Leemos todo el bloque de golpe (más eficiente que byte a byte)
    if (fread(str, 1, len + 1, input) != len + 1) {
        free(str); // Limpiar en caso de error
        return NULL;
    }

    str[len] = '\0'; // Asegurarnos de que el caracter nulo está en la posición final

    return str; // Devolvemos el puntero al string creado dinámicamente
}

int main(int argc, char *argv[])
{
	FILE* file=NULL;
    char *str;

	if (argc < 2) {
        fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
        exit(1);
    }

    // rb = lectura en modo binario (importante en Windows para no convertir \n a \r\n automágicamente)
    if ((file = fopen(argv[1], "rb")) == NULL) {
        perror("The input file could not be opened");
        return 2;
    }
    
    // Ejecuta loadstr() y asigna a str hasta que devuelva NULL (fin del archivo o error)
    while((str = loadstr(file)) != NULL) {
        printf("%s\n", str);
        free(str); // Obligatorio liberar cada string devuelta por loadstr
    }

    fclose(file);
    return 0;
}
