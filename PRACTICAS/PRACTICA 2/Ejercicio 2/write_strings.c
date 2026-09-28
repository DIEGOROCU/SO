#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int main(int argc, char* argv[])
{
    FILE* file=NULL;
    size_t ret;

    if (argc < 3) {
        fprintf(stderr,"Usage: %s <file_name> <string> [string ...]\n",argv[0]);
        exit(1);
    }

    if ((file = fopen(argv[1], "w")) == NULL) {
        err(2,"The output file %s could not be opened",argv[1]);
    }

    for (int i = 2; i < argc; i++) {
        char* string = argv[i];

        ret = fwrite(string, strlen(string) + 1, 1, file);

        if (ret != 1){
            fclose(file);
            err(3,"fwrite() failed!!");
        }

    }

    fclose(file);

	return 0;
}