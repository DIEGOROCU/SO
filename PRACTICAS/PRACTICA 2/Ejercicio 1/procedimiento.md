# Procedimiento - Ejercicio 1

## Compilacion

En Linux, desde la carpeta del ejercicio:

```bash
make
```

Tambien se puede compilar directamente con `gcc`:

```bash
gcc -Wall -g show_file.c -o show_file
```

Para limpiar los ejecutables y ficheros objeto generados:

```bash
make clean
```

## Pruebas

Se puede probar pasando el propio fichero de codigo fuente como argumento y comparando la salida con el original:

```bash
./show_file show_file.c > output.txt
diff -u show_file.c output.txt
```

Si no hay diferencias, el comportamiento es identico al original.

Tambien se puede validar la solucion con el script de comprobacion provisto:

```bash
bash "../Codigos C/ejercicio1/check_showfile"
```

Salida esperada:
```text
Everything seems ok!
```

## Modificaciones realizadas

1. Se sustituyo la lectura caracter a caracter mediante `getc()` por `fread(&c, sizeof(c), 1, file)`:
   - Se cambia el tipo de la variable `c` de `int` a `unsigned char`.
   - La condicion del bucle pasa de `(c = getc(file)) != EOF` a `fread(&c, sizeof(c), 1, file) == 1`.
2. Se sustituyo la escritura por pantalla con `putc()` por `fwrite(&c, sizeof(c), 1, stdout)`:
   - Se comprueba que el valor de retorno devuelto por `fwrite` sea `1` (numero de elementos escritos). En caso contrario, se emite error con `err()`.
3. Se añadio la cabecera `<err.h>` para el uso estandar de la funcion de diagnostico `err()`.
