#include <stdio.h>
#include <stdlib.h>

char *loadstr(FILE *input)
{
    unsigned char c;
    size_t len = 0;
    long pos;
    char *str;
    pos = ftell(input);
    if (pos == -1L) {
        return NULL;
    }

    while (1) {
        if (fread(&c, sizeof(c), 1, input) != 1) {
            return NULL;
        }
        if (c == '\0') {
            break;
        }
        len++;
    }

    if (fseek(input, pos, SEEK_SET) != 0) {
        return NULL;
    }

    str = malloc(len + 1);
    if (str == NULL) {
        return NULL;
    }

    if (fread(str, 1, len + 1, input) != len + 1) {
        free(str);
        return NULL;
    }

    str[len] = '\0';

    return str;
}

int main(int argc, char *argv[])
{
	FILE* file=NULL;
    char *str;

	if (argc < 2) {
        fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
        exit(1);
    }

    if ((file = fopen(argv[1], "rb")) == NULL) {
        perror("The input file could not be opened");
        return 2;
    }
    
    while((str = loadstr(file)) != NULL) {
        printf("%s\n", str);
        free(str);
    }

    fclose(file);
    return 0;
}
