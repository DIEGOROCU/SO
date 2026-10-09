#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define LABEL_MAX_LEN 15

// Struct encapsula datos lógicos que pueden ocupar más de un byte.
typedef struct {
    int id; // Típicamente 4 bytes
    double value; // Típicamente 8 bytes
    char label[LABEL_MAX_LEN + 1]; // Array fijo. Padding extra puede ser inyectado por el compilador
} SimpleRecord;

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    SimpleRecord record; // Reserva memoria en pila del tamaño total del struct (+ padding)

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    // "rb" (read binary). Crítico aquí, pues el archivo fue dumpeado directamente desde la memoria
    if ((file = fopen(argv[1], "rb")) == NULL) {
        err(2, "The input file %s could not be opened", argv[1]);
    }

    // Leemos el sizeof(SimpleRecord) como un bloque en crudo.
    // Esto funciona solo si la arquitectura, compilador y padding son idénticos que los de escritura.
    while (fread(&record, sizeof(SimpleRecord), 1, file) == 1) {
        printf("ID:%d, Valor:%.2f, Etiqueta: '%s'\n",
               record.id, record.value, record.label);
    }

    fclose(file);
    return 0;
}
