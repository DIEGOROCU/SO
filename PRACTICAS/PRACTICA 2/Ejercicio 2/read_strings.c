#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

char *loadstr(FILE *input)
{
    unsigned char c;
    size_t len = 0;
    long pos;
    char *str;

    pos = ftell(input);   // guarda posición actual

    while (fread(&c, sizeof(c), 1, input) == 1 && c != '\0') {
        len++;
    }

    if (fseek(input, pos, SEEK_SET) != 0) {
        return NULL;
    }

    str = malloc(len + 1);
    if (str == NULL) {
        return NULL;
    }

    if (fread(str, 1, len, input) != len) {
        free(str);
        return NULL;
    }

    str[len] = '\0';

    fseek(input, 1, SEEK_CUR);   // saltar el '\0'

    return str;
}

int main(int argc, char *argv[])
{
	FILE* file=NULL;
    unsigned char c;
    size_t ret;

	if (argc < 2) {
        fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
        exit(1);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        err(2,"The input file %s could not be opened",argv[1]);
    }
        
    
}
