#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Cabecera necesaria para usar getopt y sus variables (optind, optarg)

void print_help() {
    printf("Usage: ./getopt [ options ] title\n");
    // ... omitimos prints por brevedad, el original es igual
    printf("  -h: display this help message\n");
    printf("  -e: print even numbers instead of odd (default)\n");
    printf("  -l length: length of the sequence to be printed\n");
    printf("  title: name of the sequence to be printed\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int is_even = 0; // Variable bandera (flag) manual
    int length = 10; 
    char *title = NULL; // Puntero nulo hasta que extraigamos el título

    // getopt itera los argumentos buscando 'h', 'e', 'l'. Los : indican que 'l' exige un valor adyacente (-l 5)
    while ((opt = getopt(argc, argv, "hel:")) != -1) {
        switch (opt) {
            case 'h':
                print_help();
                return 0;
            case 'e':
                is_even = 1;
                break;
            case 'l':
                // optarg es seteado por getopt apuntando al valor (string). atoi() convierte ASCII a Int
                length = atoi(optarg);
                break;
            default:
                print_help();
                return 1;
        }
    }

    // optind guarda el índice del primer argumento que NO es una opción de getopt (en este caso, title)
    if (optind < argc) {
        title = argv[optind];
    } else {
        printf("Error: Missing title argument.\n");
        print_help();
        return 1;
    }

    printf("Title: %s\n", title);
    
    int current;
    if (is_even == 1) {
        current = 2;
    } else {
        current = 1;
    }

    for (int i = 0; i < length; i++) {
        printf("%d ", current);
        current += 2;
    }
    printf("\n");

    return 0;
}
