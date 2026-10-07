#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define LABEL_MAX_LEN 15
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

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

    if ((file = fopen(argv[1], "r")) == NULL) {
        err(2, "The input file %s could not be opened", argv[1]);
    }

    while (fscanf(file, "%d %lf %" STR(LABEL_MAX_LEN) "s",
                  &record.id, &record.value, record.label) == 3) {
        printf("ID:%d, Valor:%.2f, Etiqueta: '%s'\n",
               record.id, record.value, record.label);
    }

    fclose(file);
    return 0;
}
