#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int main(int argc, char* argv[])
{
    FILE* file=NULL;
    size_t ret;

    if (argc < 3) {
        // En C los arreglos de chars usan argv. [0] es nombre programa, [1] es file_name, del [2] en adelante son las strings.
        fprintf(stderr,"Usage: %s <file_name> <string> [string ...]\n",argv[0]);
        exit(1);
    }

    // "w" sobreescribe (trunca) el archivo si existe, o lo crea si no existe
    if ((file = fopen(argv[1], "w")) == NULL) {
        err(2,"The output file %s could not be opened",argv[1]);
    }

    // Iteramos por todos los strings introducidos por el usuario
    for (int i = 2; i < argc; i++) {
        char* string = argv[i];

        // strlen cuenta chars sin incluir '\0'. +1 asegura que se escriba el byte '\0' delimitador en el fichero.
        ret = fwrite(string, strlen(string) + 1, 1, file);

        if (ret != 1){
            fclose(file);
            err(3,"fwrite() failed!!");
        }
    }

    fclose(file);

	return 0;
}