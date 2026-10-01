# Respuestas - Ejercicio 3

## Parte C - Comparacion y analisis

### 1. Comparacion de tamaños entre `mis_datos.txt` y `mis_datos.bin`

- **Tamaño de `mis_datos.txt`**: 48 bytes.
- **Tamaño de `mis_datos.bin`**: 96 bytes.

`mis_datos.bin` es el doble de grande que `mis_datos.txt` (96 bytes frente a 48 bytes). Las razones de esta diferencia son:

1. **Almacenamiento de cadenas (`label`)**:
   - En **texto**, las cadenas solo ocupan la longitud de sus caracteres visibles mas los espacios delimitadores (p. ej., `"Madrid"` ocupa 6 bytes).
   - En **binario**, el campo `label` es un array estatico de tamaño fijo `LABEL_MAX_LEN + 1` = 16 bytes. Cada registro reserva e imprime los 16 bytes completos en disco, rellenando con ceros (`\0`) los bytes sobrantes.
2. **Alineacion y padding en memoria (binario)**:
   - En la estructura de C, un `int` ocupa 4 bytes y un `double` ocupa 8 bytes. En arquitecturas de 64 bits (x86_64), un `double` requiere alinearse a direcciones multiplo de 8 bytes. Por ello, el compilador inserta **4 bytes de padding** (relleno) entre `id` y `value`.
   - Por tanto, cada `SimpleRecord` ocupa en binario exactamente:
     $$\text{sizeof(SimpleRecord)} = 4\,(\text{id}) + 4\,(\text{pad}) + 8\,(\text{value}) + 16\,(\text{label}) = 32\text{ bytes}$$
     Multiplicado por 3 registros, da $3 \times 32 = 96$ bytes fijos.
3. **Almacenamiento de numeros**:
   - En **texto**, los numeros se codifican como caracteres ASCII variables: `1` ocupa 1 byte, `3.10` ocupa 4 bytes, un espacio 1 byte.
   - En **binario**, los tipos numericos ocupan siempre su tamaño estandar (`sizeof(int) = 4` y `sizeof(double) = 8`), independientemente del valor que contengan. En este ejemplo particular con numeros pequenos, la representacion en texto resulta mas compacta que la binaria con padding y cadenas de tamaño fijo.

---

### 2. Comportamiento al visualizar el fichero binario con `cat`

Al ejecutar `cat mis_datos.bin`, la salida en la terminal aparece corrupta, entrecortada o con simbolos extranos (``), pudiendo incluso desconfigurar el cursor o la fuente de la terminal.

**Motivo:**
`cat` asume que el flujo de entrada contiene secuencias de texto ASCII / UTF-8 imprimibles terminadas en saltos de linea (`\n`). En `mis_datos.bin`:
- Los numeros enteros (`int`) y de punto flotante (`double`) se guardan directamente como su patron de bits en memoria (IEEE-754 para `double`, complemento a dos en little-endian para `int`). Muchos de estos bytes caen en rangos de caracteres de control (valores entre `0x00` y `0x1F` o secuencias de escape ANSI) o en codificaciones invalidas de UTF-8.
- Abundan los bytes nulos (`0x00`), tanto en el padding de alineacion como en el relleno de `label` y en los enteros.
La terminal intenta interpretar estos bytes arbitrarios como caracteres visibles o codigos de control, produciendo caracteres sustitutos o artefactos visuales.

---

### 3. Analisis del volcado hexadecimal (`xxd`) y alineacion en memoria

Al ejecutar `xxd mis_datos.bin`, se observa lo siguiente:

```text
00000000: 0100 0000 0000 0000 cdcc cccc cccc 0840  ...............@
00000010: 4261 7263 656c 6f6e 6100 0000 0000 0000  Barcelona.......
00000020: 0000 0000 0000 0000 85eb 51b8 1ec5 3340  ..........Q...3@
00000030: 4d61 6472 6964 0000 0000 0000 0000 0000  Madrid..........
00000040: 0200 0000 0000 0000 ae47 e17a 14ae 1d40  .........G.z...@
00000050: 5661 6c65 6e63 6961 0000 0000 0000 0000  Valencia........
```

Cada `SimpleRecord` ocupa exactamente **32 bytes** (dos lineas de 16 bytes en la salida de `xxd`):

1. **Registro 1** (offset `0x00000000` a `0x0000001F`):
   - `01 00 00 00`: Campo `id = 1` (`int`, 4 bytes en little-endian: `0x00000001`).
   - `00 00 00 00`: **4 bytes de padding** introducidos por el compilador para alinear el siguiente campo `double` a un multiplo de 8 bytes.
   - `cd cc cc cc cc cc 08 40`: Campo `value = 3.10` (`double`, 8 bytes en formato IEEE-754 little-endian).
   - `42 61 72 63 65 6c 6f 6e 61 00 ...`: Campo `label` (16 bytes en total). Contiene los caracteres ASCII de `"Barcelona\0"` y 6 bytes nulos de relleno hasta completar los 16 bytes.
2. **Registro 2** (offset `0x00000020` a `0x0000003F`):
   - `00 00 00 00`: Campo `id = 0`.
   - `00 00 00 00`: Padding (4 bytes).
   - `85 eb 51 b8 1e c5 33 40`: Campo `value = 19.77`.
   - `4d 61 64 72 69 64 00 ...`: Campo `label` (`"Madrid\0"` + 10 bytes nulos de relleno).
3. **Registro 3** (offset `0x00000040` a `0x0000005F`):
   - `02 00 00 00`: Campo `id = 2`.
   - `00 00 00 00`: Padding (4 bytes).
   - `ae 47 e1 7a 14 ae 1d 40`: Campo `value = 7.42`.
   - `56 61 6c 65 6e 63 69 61 00 ...`: Campo `label` (`"Valencia\0"` + 7 bytes nulos de relleno).

La representacion coincide exactamente con el diseño en memoria de la estructura en la arquitectura x86_64, reflejando el orden de bytes (little-endian) y el padding requerido por las reglas de alineacion natural de hardware.

---

### 4. Ventajas y desventajas de cada formato

| Formato | Ventajas | Desventajas |
| :--- | :--- | :--- |
| **Texto** | **Legibilidad e interoperabilidad:** Es legible y editable directamente por humanos mediante herramientas estandar (`cat`, `less`, editores de texto). Es independiente de la arquitectura hardware (no le afecta el endianness ni la alineacion interna de la maquina). | **Sobrecarga de computo y espacio:** Requiere parsing y conversion continua de cadenas a numeros binarios y viceversa (`fscanf`, `fprintf`). Al ser las lineas de longitud variable, no permite acceso aleatorio eficiente con `fseek` (no se puede calcular directamente la posicion del registro $N$). |
| **Binario** | **Rendimiento y acceso directo:** Copia directa entre memoria y disco con `fread`/`fwrite` sin necesidad de conversion de tipos. Al tener cada registro un tamaño fijo (`sizeof(SimpleRecord) = 32`), permite acceso aleatorio inmediato al registro $N$ mediante `fseek(f, N * sizeof(SimpleRecord), SEEK_SET)`. | **Falta de portabilidad y no legible:** Incompatible entre sistemas con distinta representacion (diferente endianness, tamaño de tipos o padding de compilador). Requiere herramientas especificas para su inspeccion o depuracion. |

---

### 5. Variante con puntero dinamico: `char *label`

Considerando la estructura modificada:
```c
typedef struct {
    int id;
    double value;
    char* label;
} SimpleRecord;
```

#### ¿Se podria escribir el array en binario con una unica llamada a `fwrite()`?
**No.** 
- **Razon:** Al llamar a `fwrite(records, sizeof(SimpleRecord), N, file)`, lo que se escribe en el fichero es el contenido bruto de la estructura en memoria. En este caso, el campo `label` almacena un **puntero** (una direccion de memoria virtual de 64 bits/8 bytes, p. ej. `0x55a1b2c3d000`). En el fichero se grabaria el valor numerico de esa direccion de memoria, mientras que los caracteres de la cadena (ubicados en el heap tras `malloc`) no se escribirian.
- Esa direccion de memoria carece de validez cuando el programa termina o se ejecuta en otro proceso/maquina.
- **Mecanismo propuesto para escritura correcta:**
  Se debe serializar cada registro elemento a elemento:
  1. Escribir los campos numericos fijos (`id` y `value`).
  2. Para `label`:
     - Opcion recomendada: Escribir primero la longitud de la cadena (`uint32_t` o `size_t len = strlen(label);`) con `fwrite(&len, sizeof(len), 1, f)` y a continuacion los bytes de la cadena con `fwrite(label, sizeof(char), len, f)`.
     - Alternativa: Escribir la cadena terminada en `\0` con `fwrite(label, strlen(label) + 1, 1, f)` (como se hizo en el Ejercicio 2).

#### ¿Se podria leer el array de golpe con una unica llamada a `fread()`?
**No.**
- **Razon:** Al hacer `fread(records, sizeof(SimpleRecord), N, file)`, `fread` simplemente sobreescribiria los bytes de las estructuras en la memoria del programa. El campo `label` recibiria la direccion de memoria antigua grabada en el fichero (puntero colgante o invalido), sin haber invocado a `malloc()` ni haber leido los caracteres de la cadena desde el fichero. Acceder a `record.label` provocaria un fallo de segmentacion (*Segmentation Fault*) o corrupcion de memoria.
- **Mecanismo propuesto para lectura correcta:**
  Se debe deserializar registro a registro:
  1. Leer los campos fijos `id` y `value` con `fread`.
  2. Leer la longitud de la cadena almacenada `len`.
  3. Reservar dinamicamente la memoria necesaria en el heap: `record.label = malloc(len + 1);`. Comprobar que no devuelve `NULL`.
  4. Leer los caracteres de la cadena del fichero en `record.label` con `fread(record.label, sizeof(char), len, f)`.
  5. Asignar el caracter terminador nulo: `record.label[len] = '\0'`.
  6. (Si se uso la alternativa con `\0`, usar una funcion como `loadstr` del Ejercicio 2 para leer caracter a caracter hasta `\0` y alojar memoria con `malloc`).
