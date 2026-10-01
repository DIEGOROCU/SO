# Procedimiento - Ejercicio 3

## Compilacion

En Linux, desde esta carpeta:

```bash
make
```

Tambien se pueden compilar individualmente:

```bash
gcc -Wall -g write_records_text.c -o write_records_text
gcc -Wall -g read_records_text.c -o read_records_text
gcc -Wall -g write_records_bin.c -o write_records_bin
gcc -Wall -g read_records_bin.c -o read_records_bin
gcc -Wall -g conversion.c -o conversion
```

Para limpiar los ejecutables y ficheros objeto:

```bash
make clean
```

---

## Parte A - Ficheros de texto

### Escritura: `write_records_text`

El programa escribe un array estatico de 3 registros `SimpleRecord` en formato texto legible por humanos, formateando cada registro como `id valor etiqueta\n` con `fprintf`:

```bash
./write_records_text mis_datos.txt
```

Salida esperada por consola:
```text
La escritura ha finalizado correctamente
```

Contenido generado en `mis_datos.txt`:
```bash
cat mis_datos.txt
```
```text
1 3.10 Barcelona
0 19.77 Madrid
2 7.42 Valencia
```

### Lectura: `read_records_text`

Lee los registros con `fscanf` en un bucle hasta alcanzar EOF o fallo de formato, limitando el ancho del campo de cadena a 15 caracteres (`%15s`) para evitar desbordamiento de buffer:

```bash
./read_records_text mis_datos.txt
```

Salida esperada:
```text
ID:1, Valor:3.10, Etiqueta: 'Barcelona'
ID:0, Valor:19.77, Etiqueta: 'Madrid'
ID:2, Valor:7.42, Etiqueta: 'Valencia'
```

---

## Parte B - Ficheros binarios

### Escritura: `write_records_bin`

Escribe el mismo array de registros directamente en binario con `fwrite`, volcando la estructura completa en memoria (`sizeof(SimpleRecord)`) elemento a elemento:

```bash
./write_records_bin mis_datos.bin
```

Salida esperada:
```text
La escritura ha finalizado correctamente
```

### Lectura: `read_records_bin`

Lee registro a registro directamente a una variable local con `fread(&record, sizeof(SimpleRecord), 1, file)` hasta que no queden registros completos:

```bash
./read_records_bin mis_datos.bin
```

Salida esperada:
```text
ID:1, Valor:3.10, Etiqueta: 'Barcelona'
ID:0, Valor:19.77, Etiqueta: 'Madrid'
ID:2, Valor:7.42, Etiqueta: 'Valencia'
```

---

## Parte C - Comparacion y analisis

Comprobacion y comparacion de los ficheros generados:

```bash
ls -l mis_datos.txt mis_datos.bin
cat mis_datos.txt
cat mis_datos.bin
xxd mis_datos.bin
```

- `mis_datos.txt` ocupa 48 bytes (caracteres ASCII, espacios y saltos de linea).
- `mis_datos.bin` ocupa 96 bytes (3 registros de 32 bytes cada uno debido a la alineacion y padding de `SimpleRecord`).
- Al ejecutar `cat mis_datos.bin`, la salida no es legible debido a los bytes no imprimibles correspondientes a los enteros, doubles y bytes nulos de relleno.
- Al ejecutar `xxd mis_datos.bin`, se identifican claramente los bloques de 32 bytes de cada estructura, su padding de 4 bytes entre `id` y `value`, la representacion IEEE-754 de 8 bytes de `value` y el array fijo de 16 bytes para `label`.

---

## Parte D - Conversion de formatos

El programa `conversion.c` permite convertir ficheros entre formatos texto y binario usando `getopt` para procesar las opciones `-i` y `-o`.

### Sintaxis

```bash
./conversion [-i t|b] [-o t|b] fichero_entrada fichero_salida
```

- `-i`: Formato de entrada (`t` para texto, `b` para binario). Por defecto: `t`.
- `-o`: Formato de salida (`t` para texto, `b` para binario). Por defecto: `t`.
- `fichero_entrada` y `fichero_salida`: Rutas de los ficheros correspondientes. Se admite `-` para representar la entrada estandar (`stdin`) o la salida estandar (`stdout`).

### Pruebas de conversion

1. **Texto a Binario:**
```bash
./conversion -i t -o b mis_datos.txt converted.bin
./read_records_bin converted.bin
cmp mis_datos.bin converted.bin
```

2. **Binario a Texto:**
```bash
./conversion -i b -o t mis_datos.bin converted.txt
diff -u mis_datos.txt converted.txt
```

3. **Valores por defecto (Texto a Texto):**
```bash
./conversion mis_datos.txt converted_default.txt
diff -u mis_datos.txt converted_default.txt
```

4. **Uso de flujos estandar (`stdin` / `stdout` con `-`):**
```bash
cat mis_datos.txt | ./conversion -i t -o b - - | ./conversion -i b -o t - -
```
Produce por la salida estandar el texto original identico tras haber pasado por conversion a binario y reconversion a texto en memoria mediante tuberias (pipes).
