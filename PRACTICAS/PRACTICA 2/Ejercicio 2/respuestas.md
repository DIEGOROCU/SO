# Respuestas - Ejercicio 2

## Funcionamiento de `write_strings`

`write_strings` abre el fichero en modo binario y escribe cada argumento con:

```c
fwrite(string, strlen(string) + 1, 1, file);
```

El `+1` incluye el byte `\0`, que permite distinguir una cadena de la siguiente.

## Funcionamiento de `loadstr`

`loadstr` comienza en la posicion actual del fichero, cuenta los bytes hasta encontrar `\0`, vuelve a la posicion inicial con `fseek`, reserva `len + 1` bytes y lee la cadena junto con su terminador. Si no encuentra el terminador, falla una reserva de memoria o no puede reposicionarse o leer, devuelve `NULL`.

Tras leer el terminador, la posicion del fichero queda preparada para cargar la siguiente cadena.
