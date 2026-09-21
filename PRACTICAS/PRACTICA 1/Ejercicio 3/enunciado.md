En este ejercicio, trabajaremos el uso de getopt() una herramienta esencial para el procesado de opciones en línea de comando. El objetivo del ejercicio es completar el código del fichero getopt.c para que sea capaz de procesar las opciones -e y -l tal y como indica el uso del programa, que puede consultarse con la opción -h:

```bash
$ make
$ ./getopt -h
Usage: ./getopt [ options ] title

options:
-h: display this help message
-e: print even numbers instead of odd (default)
-l length: length of the sequence to be printed
title: name of the sequence to be printed
```

Una vez completado, el programa deberá imprimir una secuencia de length números (10 por defecto; podemos cambiarlo con la opción -l) impares (por defecto) o pares si se incluye la opción -e. Los argumentos -l length y -e son opcionales, pero el argumento title siempre debe estar presente en la línea de comando.

Es necesario familiarizarse con la función getopt() consultando la página de manual de getopt(): man 3 getopt

Para completar el código, incluye las opciones -l y -e en la llamada a getopt() y completa la estructura switch-case para modificar los valores por defecto de la variable options. Para leer el valor numérico asociado a la opción -l, deberás utilizar la variable global optarg, teniendo en cuenta que esta variable es una cadena de caracteres (tipo char *) y, sin embargo, queremos almacenar la opción como un número entero (tipo int). Consulta el uso de la función strtol() en el manual (man 3 strtol) para saber cómo realizar esa conversión.

Asimismo, dado que el argumento title no será procesado por getopt() (pues no está precedido por una marca de opción al estilo -l), deberemos continuar el procesamiento de la cadena de entrada tras el bucle for. Para ello, se usará la variable optind junto con argv para almacenar el valor de la cadena de caracteres que será el título de nuestra secuencia.

Completa el código y responde a las siguientes preguntas:

1. ¿Qué cadena de caracteres debes utilizar como tercer argumento de getopt()?
2. ¿Qué línea de código utilizas para leer el argumento title?
