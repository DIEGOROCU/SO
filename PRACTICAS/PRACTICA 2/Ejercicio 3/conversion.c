#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

#define LABEL_MAX_LEN 15
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

typedef struct {
    int id;
    double value;
    char label[LABEL_MAX_LEN + 1];
} SimpleRecord;

/* Funciones auxiliares para formato texto */
int read_record_text(FILE *f, SimpleRecord *r)
{
    memset(r, 0, sizeof(SimpleRecord));
    int ret = fscanf(f, "%d %lf %" STR(LABEL_MAX_LEN) "s",
                     &r->id, &r->value, r->label);
    if (ret == 3) {
        return 1;
    }
    if (ret == EOF || feof(f)) {
        return 0;
    }
    return -1;
}

int write_record_text(FILE *f, const SimpleRecord *r)
{
    if (fprintf(f, "%d %.2f %s\n", r->id, r->value, r->label) < 0) {
        return -1;
    }
    return 1;
}

/* Funciones auxiliares para formato binario */
int read_record_bin(FILE *f, SimpleRecord *r)
{
    size_t n = fread(r, sizeof(SimpleRecord), 1, f);
    if (n == 1) {
        return 1;
    }
    if (feof(f)) {
        return 0;
    }
    return -1;
}

int write_record_bin(FILE *f, const SimpleRecord *r)
{
    if (fwrite(r, sizeof(SimpleRecord), 1, f) != 1) {
        return -1;
    }
    return 1;
}

/* Funcion de conversion general */
int convert_records(FILE *in, char in_fmt, FILE *out, char out_fmt)
{
    SimpleRecord record;
    int status;

    while (1) {
        if (in_fmt == 't') {
            status = read_record_text(in, &record);
        } else {
            status = read_record_bin(in, &record);
        }

        if (status == 0) {
            break; /* Fin de fichero */
        }
        if (status < 0) {
            warnx("Error al leer registro del fichero de entrada");
            return -1;
        }

        int wstatus;
        if (out_fmt == 't') {
            wstatus = write_record_text(out, &record);
        } else {
            wstatus = write_record_bin(out, &record);
        }

        if (wstatus < 0) {
            warnx("Error al escribir registro en el fichero de salida");
            return -1;
        }
    }
    return 0;
}

static void print_usage(const char *progname)
{
    fprintf(stderr, "Usage: %s [-i t|b] [-o t|b] <input_file> <output_file>\n", progname);
}

int main(int argc, char *argv[])
{
    int opt;
    char in_fmt = 't';
    char out_fmt = 't';

    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i':
                if (strlen(optarg) != 1 || (optarg[0] != 't' && optarg[0] != 'b')) {
                    fprintf(stderr, "Error: el formato de entrada (-i) debe ser 't' o 'b'\n");
                    print_usage(argv[0]);
                    exit(1);
                }
                in_fmt = optarg[0];
                break;
            case 'o':
                if (strlen(optarg) != 1 || (optarg[0] != 't' && optarg[0] != 'b')) {
                    fprintf(stderr, "Error: el formato de salida (-o) debe ser 't' o 'b'\n");
                    print_usage(argv[0]);
                    exit(1);
                }
                out_fmt = optarg[0];
                break;
            default:
                print_usage(argv[0]);
                exit(1);
        }
    }

    if (argc - optind != 2) {
        print_usage(argv[0]);
        exit(1);
    }

    const char *in_path = argv[optind];
    const char *out_path = argv[optind + 1];

    FILE *in_file = NULL;
    FILE *out_file = NULL;

    /* Soporte para stdin/stdout si se pasa "-" */
    if (strcmp(in_path, "-") == 0) {
        in_file = stdin;
    } else {
        in_file = fopen(in_path, (in_fmt == 'b') ? "rb" : "r");
        if (in_file == NULL) {
            err(2, "No se pudo abrir el fichero de entrada '%s'", in_path);
        }
    }

    if (strcmp(out_path, "-") == 0) {
        out_file = stdout;
    } else {
        out_file = fopen(out_path, (out_fmt == 'b') ? "wb" : "w");
        if (out_file == NULL) {
            if (in_file != stdin) {
                fclose(in_file);
            }
            err(2, "No se pudo abrir el fichero de salida '%s'", out_path);
        }
    }

    int res = convert_records(in_file, in_fmt, out_file, out_fmt);

    if (in_file != stdin) {
        fclose(in_file);
    }
    if (out_file != stdout) {
        fclose(out_file);
    }

    if (res != 0) {
        exit(3);
    }

    return 0;
}
