#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
    FILE* file=NULL;
    unsigned char c;
    size_t ret;

    if (argc!=2) {
        fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
        exit(1);
    }

    /* Open file */
    if ((file = fopen(argv[1], "r")) == NULL)
        err(2,"The input file %s could not be opened",argv[1]);

    /* Read file byte by byte */
    while (fread(&c, sizeof(c), 1, file) == 1) {
        /* Print byte to stdout */
        ret=fwrite(&c, sizeof(c), 1, stdout);

        if (ret != 1){
            fclose(file);
            err(3,"fwrite() failed!!");
        }
    }

    fclose(file);
    return 0;
}