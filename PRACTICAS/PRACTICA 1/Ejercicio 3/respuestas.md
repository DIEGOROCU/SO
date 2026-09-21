# Respuestas - Ejercicio 3

**1. ¿Qué cadena de caracteres debes utilizar como tercer argumento de getopt()?**
Se debe utilizar la cadena `"hel:"`.
*   `h`: para la opción `-h` (ayuda), sin argumentos.
*   `e`: para la opción `-e` (números pares), sin argumentos.
*   `l:`: para la opción `-l` (longitud), el sufijo `:` indica que requiere un argumento obligatorio.

*(Nota: si no se desea incluir la opción de ayuda en getopt, la cadena podría ser simplemente `"el:"`)*.

**2. ¿Qué línea de código utilizas para leer el argumento `title`?**
Tras evaluar todas las opciones con `getopt` dentro del bucle, se usa la variable global `optind` (que contiene el índice del siguiente argumento de `argv` a procesar).
La línea de código sería:
```c
title = argv[optind];
```
*(Es conveniente comprobar antes que `optind < argc` para asegurar que el usuario ha proporcionado el argumento).*
