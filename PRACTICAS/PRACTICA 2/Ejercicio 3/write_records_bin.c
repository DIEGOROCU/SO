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

    if ((file = fopen(argv[1], "wb")) == NULL) {
        err(2, "The output file %s could not be opened", argv[1]);
    }

    for (size_t i = 0; i < NUM_RECORDS; i++) {
        if (fwrite(&records[i], sizeof(SimpleRecord), 1, file) != 1) {
            fclose(file);
            err(3, "fwrite() failed!!");
        }
    }

    fclose(file);
    printf("La escritura ha finalizado correctamente\n");
    return 0;
}
