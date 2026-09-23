# Respuestas a los ejercicios teóricos - Ejercicio 1

## 1. Compilación
Para llevar a cabo las pruebas de este apartado, los comandos a utilizar en la terminal son:
```bash
gcc hello.c -o hello
./hello
gcc -E hello2.c -o hello2.i
gcc --save-temps hello2.c
```
* **¿Qué ha ocurrido con la “llamada a min()” en hello2.i?**
Al tratarse de una macro definida con `#define`, el preprocesador la ha sustituido por el código en línea `((a)<(b)?(a):(b))` antes de compilar.
* **¿Qué efecto ha tenido la directiva #include <stdio.h>?**
Ha copiado todo el contenido de la cabecera `stdio.h` directamente en el archivo `hello2.i`.

## 2. Herramienta make
Comandos utilizados:
```bash
make
touch aux.c
make clean
```
* **Marca el fichero aux.c como modificado ejecutando touch aux.c. Después ejecuta de nuevo make. ¿Qué diferencia hay? ¿Por qué?**
Sólo se recompila `aux.c` (generando un nuevo `aux.o`) y se enlaza el ejecutable final, en lugar de recompilar todos los archivos `.c`. Esto es porque `make` mira las fechas de modificación y sólo trabaja sobre lo que ha cambiado o depende de ello.
* **¿Qué ha sucedido con make clean? Observa que está marcado como phony...**
Borra los ficheros compilados `.o` y ejecutables. Se marca como `.PHONY` para que `make` sepa que "clean" es el nombre de una tarea y no de un archivo. Si creas un archivo llamado `clean` y quitas el `.PHONY`, al ejecutar `make clean` dirá que está actualizado y no borrará nada.
* **Comenta la línea LIBS = -lm y compila. ¿Qué sucede? ¿Qué etapa da problemas?**
Falla la etapa de **enlazado (link)** con un "undefined reference", porque al quitar `-lm` el compilador no sabe dónde encontrar el código de las funciones matemáticas que hemos usado.

## 3. Tamaño de variables
*(Nota para los siguientes apartados: los ficheros `.c` se compilan individualmente con comandos como `gcc main1.c -o main1` y se ejecutan con `./main1`)*

* **main1.c: ¿Por qué el primer printf() imprime valores distintos para 'a' con %d y %c?**
`%c` imprime el símbolo ASCII, y `%d` imprime el valor numérico en la tabla ASCII.
* **¿Cuánto ocupa un tipo de datos char?**
1 byte.
* **¿Por qué el valor de 'a' cambia tanto al incrementarlo en 6?**
Porque un `char` con signo tiene como máximo el valor 127. Como era 122 ('z') y le sumamos 6, se pasa a 128 y ocurre un desbordamiento (*overflow*), dando la vuelta al rango numérico y tomando un valor negativo (-128).
* **Si un "long" y un "double" ocupan lo mismo, ¿por qué hay 2 tipos de datos diferentes?**
Porque `long` almacena enteros usando representación de complemento a dos, mientras que `double` almacena números con decimales (coma flotante) bajo un estándar distinto (IEEE 754).
* **main2.c: ¿Tenemos un problema de compilación o de ejecución?**
De **compilación**.
* **¿Por qué se da el problema? Soluciónalo.**
El compilador arroja un error como `variably modified ‘array2’ at file scope` (o `array bound is not an integer constant`). Al declarar un array de forma global (`int array2[a];`), su tamaño debe ser una constante evaluable en tiempo de compilación. Aquí `a` es una variable. Se soluciona poniendo `#define A 7` y usando `int array2[A];`, o creando el array dentro del `main`.
* **¿Qué significa el modificar "%lu" en printf()?**
Significa *long unsigned*. Es el formato para imprimir un valor numérico grande sin signo, como el devuelto por `sizeof()`.
* **¿A qué dirección apunta "pc"? ¿Coincide con la de alguna declarada? ¿Coinciden los tamaños?**
Apunta a la dirección de `x` (`pc = &x;`). Coinciden en apuntar a la misma dirección base, pero no en su tamaño: `sizeof(x)` será 1 (por ser char) y `sizeof(pc)` será 8 (por ser un puntero en 64 bits).
* **¿Coincide el valor del tamaño de array1 con el número de elementos? ¿Por qué?**
No necesariamente. `sizeof(array1)` devuelve los bytes que ocupa el array completo. Como cada elemento es un `int` (normalmente 4 bytes), si hay 10 elementos el tamaño devuelto es 40, no 10.
* **¿Coinciden las direcciones a las que apuntan str1 y str2?**
No, se alojan en zonas de memoria diferentes (uno es un puntero a un string literal en la zona de solo lectura, y el otro es un array copiado en memoria estática o pila).
* **¿Por qué los tamaños de str1 y str2 son diferentes?**
`str1` es un puntero (`char*`), por lo que su `sizeof` es 8 bytes. `str2` es un array, por lo que su `sizeof` es la cantidad total de letras más el nulo final (21 bytes).

## 4. Arrays

* **array1.c: ¿Por qué no es necesario escribir "&list" para obtener su dirección?**
Porque el nombre de un array por sí solo evalúa como la dirección de memoria de su primer elemento.
* **¿Qué hay almacenado en la dirección de list?**
El primer elemento del array, es decir `list[0]`.
* **¿Por qué es necesario pasar como argumento el tamaño en init_array?**
Porque al pasar un array como parámetro a una función, este decae a un puntero perdiéndose la información sobre cuántos elementos tiene, así que el bucle necesita un límite.
* **¿Por qué el tamaño devuelto por sizeof() en init_array no coincide con main()?**
En `main`, `list` es un array (`sizeof` = 20 bytes). En `init_array`, recibe un *puntero*, por lo que `sizeof` devuelve 8 bytes (tamaño del puntero).
* **¿Por qué NO es necesario pasar el tamaño en init_array2?**
Porque internamente la función usa la constante preprocesada `N` que se definió con `#define N 5` de forma global, en vez de un parámetro.
* **¿Coincide el tamaño devuelto por sizeof() en init_array2 con main()?**
No. Aunque declaremos `int array[N]`, como parámetro de función sigue decayendo a un puntero simple (`int *`), por lo que su `sizeof` es 8 y no 20.
* **array2.c: ¿La copia del array se realiza correctamente? ¿Por qué?**
No. En C, hacer `dst = src` sobre variables locales copia los punteros, no el contenido (y de hecho, ese cambio de puntero sólo afecta localmente dentro de la función). Se debe usar un bucle `for` o `memcpy`.

## 5. Punteros

* **punteros1.c: ¿Qué operador usamos para declarar un puntero?** Asterisco (`*`).
* **¿Qué operador usamos para obtener la dirección?** Ampersand (`&`).
* **¿Qué operador para acceder al contenido de una dirección (desreferenciar)?** Asterisco (`*`).
* **Hay un error en el código. ¿Se produce en compilación o en ejecución? ¿Por qué se produce?**
En **ejecución** (*Segmentation fault*). Se produce por la instrucción `ptr = (int*) 0x600a48; *ptr = 13;`, donde intentamos acceder o escribir a una dirección de memoria fija a la fuerza, la cual no está asignada al programa por el sistema operativo, resultando en un acceso inválido a memoria.
* **punteros2.c: ¿Cuántos bytes se reservan con malloc()?** 
Se reservan 127 elementos `int` (típicamente de 4 bytes) = 508 bytes.
* **¿Cuál es la dirección del primer y último byte?** 
La del primer byte es la que devuelve `malloc` (`ptr`). La del último byte es `ptr + 507 bytes` (o la dirección base + `127 * sizeof(int) - 1`).
* **¿Por qué el contenido de ptr es 7 y no 5?**
Se hizo `*ptr = 5`, pero justo después `ptr[0] = 7;`. Ambas notaciones acceden a la misma posición (el primer elemento), por lo que lo sobrescribe.
* **¿Por qué se modifica ptr[1] tras *ptr2=15;?**
Porque previamente se hizo `ptr2 = ptr; ptr2++;`. Eso hizo que el puntero `ptr2` avanzara al segundo elemento, por lo que desreferenciar `ptr2` equivale a modificar `ptr[1]`.
* **Indica dos modos de escribir el valor 13 en ptr[100]:**
1. Notación de array: `ptr[100] = 13;`
2. Notación de punteros: `*(ptr + 100) = 13;`
* **Hay un error en el código. ¿Cuál es?**
Hay un problema en ejecución o bug lógico grave: se hace `free(ptr);` y en la línea siguiente `*ptr = 3;`. Es un caso clásico de *use-after-free*, accediendo a memoria que ya hemos devuelto al sistema operativo, lo que puede corromper datos o abortar el programa.
* **punteros3.c: ¿Por qué cambia ptr[13] tras ptr = &c;?**
Porque `ptr` ahora apunta a la dirección de memoria de la única variable `c`. Intentar leer la posición 13 a partir de ahí (`ptr[13]`) lee basura u otras partes de la memoria fuera del límite de `c` (*undefined behavior*).
* **El código tiene un error. ¿Se manifiesta en compilación o ejecución?**
En **ejecución**. El programa falla al ejecutar `free(ptr)`.
* **¿Qué ocurre con la zona de malloc? ¿Se puede acceder o liberar?**
La zona de memoria de `malloc` se "pierde" en el limbo porque sobreescribimos la única variable (`ptr`) que guardaba su dirección. A partir de ese momento, no se puede acceder a ella ni liberarla, causando una **fuga de memoria** (memory leak). Y la llamada final a `free(ptr)` da un error fatal al intentar liberar `&c`, que es una variable global no generada por `malloc`.

## 6. Funciones

* **arg1.c: ¿Por qué xc no se modifica tras sumC? ¿Dónde se modifica?**
En C el paso de argumentos es "por valor". `xc` se copia para enviarlo a la función. Las modificaciones que hace `sumC` (`a.re = 12.5;`) ocurren en la memoria de la pila local de esa función sobre esa copia, dejando al `xc` original del `main` inalterado.
* **Comenta las declaraciones adelantadas de sum y sumC. ¿Qué ocurre?**
Ocurre un error o warning de compilación, porque el compilador lee de arriba abajo y al encontrar `sum` o `sumC` en el `main` asume tipos por defecto o no los reconoce.
* **arg2.c: ¿Por qué cambia y tras sum()?**
Porque a `sum` se le pasó la dirección de `y` (paso "por referencia" simulado mediante punteros: `&y`). Modificar el contenido de ese puntero afecta directamente a la variable original.
* **¿Por qué a veces se usa `.` y otras `->`?**
Se usa el punto `.` cuando es una variable directa de tipo `struct`. Se usa la flecha `->` cuando es un puntero hacia una variable de tipo `struct`.
* **¿Por qué el valor de zc pasa a ser incorrecto?**
Porque `sumC` devuelve la dirección de una variable **local** (`&r`). Al salir de la función, esa variable local se destruye/invalida y el puntero apunta a memoria basura.
* **Corrige el código:**
Para evitarlo, la memoria se debe alojar en el `main` y pasar por referencia o devolviendo la estructura entera por valor como en `arg1.c`, o utilizar `malloc` dentro de la función `sumC`.

## 7. Cadenas de caracteres (strings)

* **strings1.c: El código contiene un error. ¿Compilación o ejecución?**
En **ejecución** (Violación de segmento / Segmentation fault).
* **¿Por qué se produce? Soluciónalo.**
En `p[0] = 'H'`, se intenta modificar un string literal de solo lectura (`"Bonjour"`). Los strings definidos así se guardan en el segmento de texto o memoria de solo lectura del programa. Se soluciona comentando la línea `p[0] = 'H', p[1] = 'i', p[2] = '\0';`.
* **¿En qué dirección está la letra 'B' y la 'j'?**
La 'B' está en la dirección apuntada por `p` (ej: `0x1000`). La 'j' está 3 bytes más adelante (ej: `0x1003`).
* **Tras p=msg2;, ¿cómo podemos recuperar la dirección de "Bonjour"?**
Es imposible si no guardamos esa dirección previamente en otro puntero, ya que la hemos sobrescrito.
* **¿Por qué la longitud de p y msg2 es 2?**
Porque se insertó un byte nulo manualmente (`msg[0] = 'B', msg[1] = 'y'`... pero el problema pregunta sobre la línea 30 original si se hace el p='H','i','\0'). El tamaño real del puntero no importa, `strlen` evalúa la cadena y cuenta caracteres hasta el primer `\0`. Si tras las H y la i escribimos `\0`, `strlen` contará sólo las 2 primeras letras.
* **¿Por qué strlen() devuelve diferente a sizeof()?**
`sizeof` devuelve los bytes reservados en tiempo de compilación o el tamaño del puntero (por ej, 8). `strlen` devuelve el número de caracteres contando dinámicamente hasta el `\0`.
* **strings2.c: El código de copy no funciona, ¿por qué?**
Igual que con los arrays, pasar un puntero por valor y hacer `dst = org` solo cambia la copia local de la variable puntero, no el exterior.
* **Usa copy2(). ¿Funciona la copia?**
`copy2(&str2)` sí funcionaría para sobreescribir el puntero en el main y que ambos apunten a "original", porque usa un puntero doble para alterar el puntero original por referencia.
* **Propón una correcta de copia:**
Si lo que se quiere es copiar letra a letra a otro bloque de memoria en vez de copiar el puntero, se debería usar `strcpy(dst, org)`.
* **¿Qué hace mod()? ¿Por qué funciona?**
Resta 32 al código ASCII de cada letra. Casualmente, en la tabla ASCII la diferencia entre letras minúsculas (ej: 'a'=97) y mayúsculas ('A'=65) es exactamente 32, por lo que la función transforma el texto original (minúsculas) a mayúsculas. Funciona sobre `str3` siempre que tenga espacio asignado y copiemos correctamente.
