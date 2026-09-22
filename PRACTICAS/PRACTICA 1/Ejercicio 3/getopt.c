#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_help() {
    printf("Usage: ./getopt [ options ] title\n");
    printf("\noptions:\n");
    printf("  -h: display this help message\n");
    printf("  -e: print even numbers instead of odd (default)\n");
    printf("  -l length: length of the sequence to be printed\n");
    printf("  title: name of the sequence to be printed\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int is_even = 0; // 0 for odd, 1 for even
    int length = 10; // Default length
    char *title = NULL;

    // The third argument specifies valid options. 
    // 'h', 'e' have no arguments. 'l' has a required argument (indicated by ':')
    while ((opt = getopt(argc, argv, "hel:")) != -1) {
        switch (opt) {
            case 'h':
                print_help();
                return 0;
            case 'e':
                is_even = 1;
                break;
            case 'l':
                length = atoi(optarg);
                break;
            default:
                print_help();
                return 1;
        }
    }

    // optind is the index of the next element to be processed in argv.
    // After processing options, argv[optind] will point to the 'title' argument.
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
