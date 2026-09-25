# Ejercicio 2 - Escritura y lectura de cadenas en ficheros

## Objetivo

Desarrollar dos programas sencillos para escribir y leer un conjunto de cadenas de longitud variable terminadas por `\0`. El caracter terminador debe almacenarse en el fichero junto con el resto de caracteres de cada cadena.

## Programas

### `write_strings.c`

Debe recibir como primer argumento el nombre del fichero de destino y como argumentos siguientes las cadenas que se escribiran. Si el fichero existe, se debe sobrescribir su contenido.

Debe utilizar las funciones `fopen()`, `fclose()`, `fwrite()` y las funciones necesarias de la biblioteca estandar.

### `read_strings.c`

Debe recibir como argumento el nombre del fichero que contiene las cadenas terminadas en `\0`. Debe leerlas e imprimirlas por pantalla, una por linea.

Para implementar la lectura se debe utilizar la funcion auxiliar:

```c
char *loadstr(FILE *input);
```

`loadstr()` debe leer una cadena terminada en `\0` desde la posicion actual del fichero, reservar dinamicamente la memoria necesaria con `malloc()`, devolver la cadena y restaurar correctamente la posicion del fichero cuando sea necesario. El programa debe utilizar `fopen()`, `fclose()`, `fread()`, `fseek()` y `malloc()`.

## Comprobacion

Tras ejecutar `write_strings`, comprueba la estructura del fichero con `xxd`. Despues ejecuta `read_strings` sobre el fichero generado y verifica que aparecen todas las cadenas, cada una en una linea independiente.
