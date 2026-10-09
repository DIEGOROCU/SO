#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define LABEL_MAX_LEN 15

typedef struct {
    int id;
    double value;
    char label[LABEL_MAX_LEN + 1];
} SimpleRecord;

static const SimpleRecord records[] = {
    {1, 3.10, "Barcelona"},
    {0, 19.77, "Madrid"},
    {2, 7.42, "Valencia"}
};

#define NUM_RECORDS (sizeof(records) / sizeof(records[0]))

int main(int argc, char *argv[])
{
    FILE *file = NULL;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    // "w" -> write text. Escribe cadenas de caracteres legibles.
    if ((file = fopen(argv[1], "w")) == NULL) {
        err(2, "The output file %s could not be opened", argv[1]);
    }

    // fprintf funciona igual que printf pero apunta a un flujo FILE específico en vez de a la consola (stdout)
    for (size_t i = 0; i < NUM_RECORDS; i++) {
        if (fprintf(file, "%d %.2f %s\n", records[i].id, records[i].value, records[i].label) < 0) {
            fclose(file);
            err(3, "fprintf() failed!!");
        }
    }

    fclose(file);
    printf("La escritura ha finalizado correctamente\n");
    return 0;
}
