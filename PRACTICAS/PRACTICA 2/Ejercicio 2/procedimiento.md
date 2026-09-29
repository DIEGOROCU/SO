# Procedimiento - Ejercicio 2

## Compilacion

En Linux, desde esta carpeta:

```bash
make
```

Tambien se pueden compilar directamente:

```bash
gcc -Wall -Wextra -std=c11 write_strings.c -o write_strings
gcc -Wall -Wextra -std=c11 read_strings.c -o read_strings
```

## Escritura de cadenas

El primer argumento es el fichero de salida y los siguientes son las cadenas:

```bash
./write_strings strings.bin uno "dos palabras" tres
```

Cada cadena se escribe con `fwrite`, incluyendo su byte terminador `\0`. El fichero se abre con `wb` para conservar los bytes sin conversiones.

## Lectura de cadenas

```bash
./read_strings strings.bin
```

`loadstr` guarda la posicion inicial con `ftell`, recorre el fichero hasta encontrar `\0`, vuelve a la posicion inicial con `fseek`, reserva memoria con `malloc` y lee la cadena completa con `fread`. Al leer tambien el terminador, el cursor queda situado al principio de la siguiente cadena.

## Comprobacion del formato del fichero

```bash
xxd strings.bin
```

En la salida debe aparecer un byte `00` al final de cada cadena.
