#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define LABEL_MAX_LEN 15

typedef struct {
    int id;
    double value;
    char label[LABEL_MAX_LEN + 1];
} SimpleRecord;

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    SimpleRecord record;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    if ((file = fopen(argv[1], "rb")) == NULL) {
        err(2, "The input file %s could not be opened", argv[1]);
    }

    while (fread(&record, sizeof(SimpleRecord), 1, file) == 1) {
        printf("ID:%d, Valor:%.2f, Etiqueta: '%s'\n",
               record.id, record.value, record.label);
    }

    fclose(file);
    return 0;
}
