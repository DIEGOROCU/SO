# Ejercicio 3 - Ficheros de texto y binarios

## Objetivo

Comprender las diferencias entre almacenar y recuperar datos estructurados en ficheros de texto y binarios usando la biblioteca estandar de C (`stdio.h`).

## Estructura de datos

Utiliza la siguiente estructura:

```c
#define LABEL_MAX_LEN 15

typedef struct {
    int id;
    double value;
    char label[LABEL_MAX_LEN + 1];
} SimpleRecord;
```

## Parte A - Ficheros de texto

### A.1 Escritura: `write_records_text.c`

1. Define estaticamente un array con 3 instancias de `SimpleRecord` e inicializalas con datos de ejemplo. Ninguna etiqueta debe superar `LABEL_MAX_LEN` caracteres.
2. Recibe como `argv[1]` el nombre del fichero de salida.
3. Abre el fichero con modo `"w"` y comprueba errores.
4. Escribe cada registro con `fprintf`, usando una linea con el formato `id valor etiqueta`.
5. Cierra el fichero.
6. Muestra un mensaje indicando que la escritura ha finalizado.

### A.2 Lectura: `read_records_text.c`

1. Recibe como `argv[1]` el nombre del fichero creado en A.1.
2. Abre el fichero con modo `"r"` y comprueba errores.
3. Lee los registros con `fscanf` dentro de un bucle, que debe terminar cuando no se puedan leer 3 elementos o se alcance el final del fichero.
4. Evita desbordamientos al leer `record.label`.
5. Muestra cada registro de forma legible, por ejemplo: `ID: %d, Valor: %.2f, Etiqueta: '%s'`.
6. Cierra el fichero.

## Parte B - Ficheros binarios

### B.1 Escritura: `write_records_bin.c`

1. Define el mismo array estatico de 3 registros usado en A.1.
2. Recibe como `argv[1]` el nombre del fichero binario de salida.
3. Abre el fichero para escritura y comprueba errores.
4. Escribe cada estructura directamente con `fwrite`, usando `sizeof(SimpleRecord)` y un elemento cada vez. Comprueba el valor devuelto por `fwrite`.
5. Cierra el fichero.
6. Muestra un mensaje indicando que la escritura ha finalizado.

### B.2 Lectura: `read_records_bin.c`

1. Recibe como `argv[1]` el nombre del fichero binario.
2. Abre el fichero para lectura y comprueba errores.
3. Declara una variable local de tipo `SimpleRecord`.
4. Lee un registro cada vez con `fread`, hasta que no se pueda leer un elemento completo.
5. Muestra los campos de cada registro de forma legible.
6. Cierra el fichero.

## Parte C - Comparacion y analisis

Ejecuta todos los programas y utiliza los siguientes comandos:

```text
ls -l mis_datos.txt mis_datos.bin
cat mis_datos.txt
cat mis_datos.bin
xxd mis_datos.bin
```

Responde a estas preguntas:

1. Compara los tamaños de `mis_datos.txt` y `mis_datos.bin`. Explica la diferencia.
2. Describe que ocurre al mostrar el fichero binario con `cat` y por que no es legible.
3. Observando `xxd` o `hexdump -C`, identifica aproximadamente el comienzo y final de cada `SimpleRecord` y relaciona la salida con la alineacion de la estructura en memoria.
4. Indica una ventaja y una desventaja del formato de texto y una ventaja y una desventaja del formato binario.
5. Considera una variante de `SimpleRecord` en la que `label` sea un `char *` reservado con `malloc`. Explica si se podria escribir o leer el array completo con una sola llamada a `fwrite()` o `fread()` y propone un mecanismo correcto.

## Parte D - Conversion de formatos

Crea `conversion.c` con esta interfaz:

```text
./conversion [-i t|b] [-o t|b] fichero_entrada fichero_salida
```

El programa debe:

1. Procesar las opciones con `getopt`.
2. Usar `-i` para indicar el formato de entrada: texto (`t`) o binario (`b`).
3. Usar `-o` para indicar el formato de salida: texto (`t`) o binario (`b`).
4. Tratar como error cualquier valor distinto de `t` o `b`.
5. Considerar texto como formato por defecto si no se proporcionan `-i` y `-o`.
6. Obtener los dos nombres de fichero restantes mediante `optind`.
7. Leer registros `SimpleRecord` del fichero de entrada y escribirlos en el fichero de salida respetando los formatos seleccionados.

Se recomienda separar el procesamiento de opciones de la funcionalidad, reutilizar el codigo de las partes A y B y factorizar el programa en funciones pequenas.
