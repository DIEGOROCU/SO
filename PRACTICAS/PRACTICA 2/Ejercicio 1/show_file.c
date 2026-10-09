#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
    FILE* file=NULL;
    unsigned char c;
    size_t ret;

    // Verificar cantidad de argumentos. argv[0] es el nombre del ejecutable.
    if (argc!=2) {
        fprintf(stderr,"Usage: %s <file_name>\n",argv[0]); // fprintf a stderr imprime en el flujo de errores
        exit(1);
    }

    /* Open file */
    // fopen devuelve un puntero a FILE. "r" abre en modo solo lectura.
    if ((file = fopen(argv[1], "r")) == NULL)
        // err() imprime mensaje, error estándar subyacente y sale con el código de estado indicado (2).
        err(2,"The input file %s could not be opened",argv[1]);

    /* Read file byte by byte */
    // fread(buffer, tamaño_elemento, cantidad, archivo) -> lee 'cantidad' de bloques de 'tamaño_elemento'
    // En este caso lee 1 byte cada vez. Retorna la cantidad de elementos leídos.
    while (fread(&c, sizeof(c), 1, file) == 1) {
        /* Print byte to stdout */
        // stdout es el flujo estándar de salida (la consola). fwrite escribe en crudo.
        ret=fwrite(&c, sizeof(c), 1, stdout);

        if (ret != 1){
            fclose(file); // Cerrar archivo antes de abortar para liberar recursos
            err(3,"fwrite() failed!!");
        }
    }

    fclose(file); // Siempre cerrar archivos abiertos para evitar memory leaks/file descriptors exhaust
    return 0;
}