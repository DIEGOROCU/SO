# Respuestas a los ejercicios teóricos - Ejercicio 1

Como no se disponía del archivo `ficheros_p1.tar.gz`, se responden las preguntas basándose en los problemas y errores clásicos de C a los que hacen referencia. Se incluyen pequeños ejemplos de código cuando es necesario para ilustrarlo.

## 1. Compilación

Para llevar a cabo las pruebas de este apartado, los comandos a utilizar en la terminal son:
```bash
# Compilar el código de forma estándar y generar un ejecutable llamado "hello"
gcc hello.c -o hello

# Ejecutar el programa resultante
./hello

# Ejecutar sólo la etapa del pre-procesador y guardar el resultado en hello2.i
gcc -E hello2.c -o hello2.i

# Alternativa: compilar y guardar a la vez todos los ficheros intermedios (.i, .s, .o)
gcc --save-temps hello2.c
```

*   **¿Qué ha ocurrido con la “llamada a min()” en hello2.i?**
    Al tratarse probablemente de una macro definida con `#define` (por ejemplo, `#define min(a,b) ((a)<(b)?(a):(b))`), la etapa de preprocesado sustituye directamente la "llamada" por el código correspondiente. No hay ninguna función `min`, sino que el código de la macro se expande "en línea" en el archivo de salida `.i`.
*   **¿Qué efecto ha tenido la directiva `#include <stdio.h>`?**
    El preprocesador de C lee el contenido del archivo de cabecera `stdio.h` (y los que este incluya) y lo copia íntegramente en el archivo `.i`, de modo que el compilador tenga todas las declaraciones de funciones como `printf` disponibles.

## 2. Herramienta make

Comandos utilizados en este apartado:
```bash
# Construye el proyecto leyendo el archivo Makefile
make

# Cambia la fecha de última modificación del fichero aux.c al instante actual
touch aux.c

# Ejecuta el objetivo "clean" definido en el Makefile para borrar archivos generados (.o, binarios)
make clean
```

*   **Marca el fichero aux.c como modificado ejecutando `touch aux.c`. Después ejecuta de nuevo `make`. ¿Qué diferencia hay? ¿Por qué?**
    Al ejecutar `make` por segunda vez, solo se recompila el fichero `aux.c` a su código objeto `aux.o` y se vuelve a enlazar (linkear) el ejecutable final. Los demás archivos `.c` no se recompilan. Esto ocurre porque `make` comprueba la fecha de modificación de los archivos; como solo `aux.c` es más reciente que el ejecutable y que `aux.o`, solo realiza las tareas estrictamente necesarias.
*   **¿Qué ha sucedido con `make clean`? Observa que está marcado como `phony`. ¿Por qué?**
    El objetivo `clean` suele eliminar los archivos `.o` y ejecutables generados. Está marcado como `.PHONY` para indicarle a `make` que no representa un archivo real en el disco. Si no fuese `.PHONY` y creásemos un archivo llamado `clean` en ese directorio, al hacer `make clean` nos diría "`make: 'clean' is up to date`" y no ejecutaría los comandos de limpieza, ya que el archivo `clean` existe y no tiene dependencias que hayan cambiado.
*   **Comenta la línea `LIBS = -lm` y compila. ¿Qué sucede? ¿Qué etapa da problemas?**
    Al quitar `-lm` (que enlaza la librería matemática `libm`), se produce un error del tipo *"undefined reference to"* (por ejemplo, al usar `sqrt` o `sin`). La etapa que da problemas es la de **enlazado (link stage)**, ya que el compilador ha convertido bien el código a objeto, pero al juntarlo para hacer el ejecutable no encuentra dónde está la definición de esas funciones matemáticas.

## 3. Tamaño de variables

*(Nota para este y los siguientes apartados: para compilar y probar de forma individual cualquiera de estos ficheros `.c`, como pide el enunciado general, el comando a utilizar en Linux/Mac/WSL es de la siguiente forma):*
```bash
# Compilar el programa (ejemplo con main1.c)
gcc main1.c -o main1

# Ejecutarlo para ver los resultados en consola
./main1
```

*   **¿Por qué el primer printf() imprime valores distintos para 'a' con %d y %c?**
    Porque `%c` interpreta el byte como un carácter ASCII e imprime el símbolo (por ejemplo, la letra 'A'), mientras que `%d` interpreta ese mismo byte como un número entero y muestra su valor numérico en la tabla ASCII (por ejemplo, 65).
*   **¿Cuánto ocupa un tipo de datos char?**
    Ocupa siempre **1 byte** (8 bits).
*   **¿Por qué el valor de 'a' cambia tanto al incrementarlo en 6?**
    Porque un `char` solo puede almacenar valores desde -128 hasta 127 (o 0 a 255 si es `unsigned`). Si se supera ese límite máximo, se produce un "desbordamiento" (overflow) y el valor numérico da la vuelta pasando a números negativos, lo que también cambia por completo el carácter ASCII representado.
*   **Si un "long" y un "double" ocupan lo mismo, ¿por qué hay 2 tipos de datos diferentes?**
    Porque su formato interno en la memoria es completamente distinto. `long` se usa para almacenar números enteros (usando complemento a 2), mientras que `double` se usa para almacenar números en coma flotante (fraccionarios/decimales) según el estándar IEEE 754, que divide los bits en signo, exponente y mantisa.

## 4. Arrays

*   **array1.c: ¿Por qué no es necesario escribir "&list" para obtener la dirección del array list?**
    En C, el nombre de un array por sí solo decae a (o se comporta como) un puntero a su primer elemento. Es decir, `list` es equivalente a `&list[0]`.
*   **¿Qué hay almacenado en la dirección de list?**
    Está almacenado el primer elemento del array.
*   **¿Por qué es necesario pasar el tamaño del array en `init_array`?**
    Porque al pasar un array a una función en C, lo que se pasa es un puntero a su primer elemento. La función pierde la información del tamaño original del array, por lo que necesita saber cuántos elementos debe inicializar/recorrer.
*   **¿Por qué el tamaño devuelto por `sizeof()` en `init_array` no coincide con el declarado en `main`?**
    En `main`, `list` es un array, por lo que `sizeof(list)` devuelve el número de elementos multiplicado por el tamaño de cada elemento. En `init_array`, lo que llega es un *puntero*, por lo que `sizeof()` devuelve el tamaño del puntero (normalmente 8 bytes en arquitecturas de 64 bits), independientemente de lo grande que sea el array.
*   **array2.c: ¿La copia del array se realiza correctamente? ¿Por qué?**
    No, hacer `array2 = array1` en C no copia los elementos. Lo que se intenta es copiar el puntero o da un error de compilación (ya que un nombre de array no es un "l-value" modificable). Para copiar un array correctamente se debe usar un bucle que copie cada elemento uno a uno, o funciones como `memcpy` de `<string.h>` (`memcpy(array2, array1, n * sizeof(tipo));`).

## 5. Punteros

*   **¿Qué operador usamos para declarar un puntero?** El asterisco: `*` (ej: `int *ptr;`).
*   **¿Qué operador usamos para obtener la dirección de una variable?** El ampersand: `&` (ej: `ptr = &var;`).
*   **¿Qué operador se utiliza para acceder al contenido de la dirección a la que apunta un puntero?** El asterisco, conocido como operador de indirección o desreferencia: `*` (ej: `*ptr = 10;`).
*   **punteros2.c: ¿Cuántos bytes se reservan con `malloc()`?** `malloc(n)` reserva `n` bytes consecutivos. Por ejemplo, `malloc(10 * sizeof(int))` reservaría normalmente 40 bytes.
*   **¿Cuál es la dirección del primer y último byte?** Si `malloc` devuelve la dirección base `P`, el primer byte está en `P` y el último en `P + (tamaño_reservado - 1)`.
*   **punteros3.c: Error típico con zonas reservadas y asignaciones.**
    Si se hace `ptr = &c;` después de haber hecho `ptr = malloc(...)`, la variable `ptr` pasa a apuntar a la dirección de memoria de la variable `c`.
    La zona de memoria original que se reservó con `malloc` se "pierde" porque ya no tenemos ningún puntero que la referencie. Esto causa una **fuga de memoria** (memory leak). Además, no se puede liberar esa zona porque no tenemos su dirección (al hacer `free(ptr)` ahora intentaríamos liberar una variable local `c`, provocando un error en ejecución (segmentation fault o abort)).

## 6. Funciones

*   **arg1.c: ¿Por qué no se modifica el valor tras `sumC`?**
    En C los argumentos se pasan siempre **por valor** (se realiza una copia local dentro de la función). Si se modifica dentro de la función, solo cambia la copia local y el original queda intacto.
*   **¿Dónde se modifica la información?**
    En el bloque de memoria de la pila (stack) correspondiente a las variables locales de esa llamada a la función.
*   **arg2.c: ¿Por qué a veces se usa `.` y otras `->` en estructuras?**
    Se usa el punto `.` cuando tenemos la estructura directamente (una variable normal de ese tipo de la estructura). Se usa la flecha `->` cuando tenemos un **puntero** a una estructura, siendo equivalente a `(*puntero).campo`.
*   **¿Por qué el valor de `zc` pasa a ser incorrecto?**
    Probablemente la función devuelva un puntero a una variable local de la función. Al salir de la función, la memoria de las variables locales se libera o queda inválida en la pila, así que usar ese puntero es comportamiento indefinido (undefined behavior) y da valores basura.

## 7. Cadenas de caracteres (strings)

*   **strings1.c: ¿En qué dirección está la letra 'B' de "Bonjour" y la 'j'?**
    Si la 'B' está en la dirección base (por ejemplo, `0x1000`), al estar codificado en C con caracteres consecutivos de 1 byte, la 'o' estaría en `0x1001`, la 'n' en `0x1002` y la 'j' en `0x1003` (la dirección de la letra inicial + 3).
*   **¿Por qué `strlen()` devuelve distinto a `sizeof()`?**
    `sizeof()` devuelve el espacio de memoria reservado en bytes para toda la variable (o en caso de un string literal como array, el tamaño de la cadena más el carácter nulo). `strlen()` es una función de la librería que cuenta los caracteres **hasta encontrar el carácter nulo de terminación `\0`**. Si tienes `char s[50] = "Hola";`, `sizeof(s)` será 50, pero `strlen(s)` será 4.
*   **strings2.c: ¿Qué hace la función `mod()`?**
    En C clásico, el operador `%` es para el módulo de enteros, pero si se escribe una función `mod`, probablemente se usa para asegurarse de que el módulo en números negativos funciona como módulo matemático y no como resto (el `%` en C puede devolver negativos, ejemplo: `-1 % 5` da `-1`).
