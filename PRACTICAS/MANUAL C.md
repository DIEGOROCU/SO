# Manual de Referencia de C y Entorno para Sistemas Operativos

Guía rápida de comandos, herramientas y conceptos clave del lenguaje C para las prácticas de Sistemas Operativos.

---

## Índice General

- [Manual de Referencia de C y Entorno para Sistemas Operativos](#manual-de-referencia-de-c-y-entorno-para-sistemas-operativos)
  - [Índice General](#índice-general)
  - [1. Compilación con GCC](#1-compilación-con-gcc)
    - [Comandos y Sintaxis Básica](#comandos-y-sintaxis-básica)
    - [Flags y Opciones del Compilador](#flags-y-opciones-del-compilador)
    - [Etapas de la Compilación](#etapas-de-la-compilación)
  - [2. Automatización con GNU Make](#2-automatización-con-gnu-make)
    - [Estructura Básica de un Makefile](#estructura-básica-de-un-makefile)
    - [Variables Convencionales](#variables-convencionales)
    - [Comandos de Make](#comandos-de-make)
    - [Objetivos Phony (.PHONY)](#objetivos-phony-phony)
  - [3. Depuración con GDB](#3-depuración-con-gdb)
    - [Preparación y Lanzamiento](#preparación-y-lanzamiento)
    - [Puntos de Ruptura (Breakpoints)](#puntos-de-ruptura-breakpoints)
    - [Control de Flujo de Ejecución](#control-de-flujo-de-ejecución)
    - [Inspección de Datos y Memoria](#inspección-de-datos-y-memoria)
    - [Diagnóstico de Fallos](#diagnóstico-de-fallos)
    - [Flujo de Depuración Paso a Paso](#flujo-de-depuración-paso-a-paso)
  - [4. Fundamentos del Lenguaje C](#4-fundamentos-del-lenguaje-c)
    - [Tipos de Datos y Operador sizeof](#tipos-de-datos-y-operador-sizeof)
    - [Modificadores de Formato en Entrada/Salida](#modificadores-de-formato-en-entradasalida)
    - [Punteros y Operadores de Dirección](#punteros-y-operadores-de-dirección)
    - [Gestión de Memoria Dinámica](#gestión-de-memoria-dinámica)
    - [Arrays y Decaimiento a Punteros](#arrays-y-decaimiento-a-punteros)
    - [Cadenas de Caracteres (Strings)](#cadenas-de-caracteres-strings)
    - [Estructuras (struct) y typedef](#estructuras-struct-y-typedef)
    - [Paso de Parámetros en Funciones](#paso-de-parámetros-en-funciones)
  - [5. Argumentos de Línea de Comandos y getopt](#5-argumentos-de-línea-de-comandos-y-getopt)
    - [argc y argv](#argc-y-argv)
    - [Función getopt](#función-getopt)
    - [Variables Globales Asociadas](#variables-globales-asociadas)
    - [Conversión de Cadenas a Números](#conversión-de-cadenas-a-números)
  - [6. Entrada/Salida y Manejo de Ficheros](#6-entradasalida-y-manejo-de-ficheros)
    - [Flujos Estándar y Apertura](#flujos-estándar-y-apertura)
    - [Lectura y Escritura](#lectura-y-escritura)
    - [Procesamiento con sscanf y strsep](#procesamiento-con-sscanf-y-strsep)
    - [Gestión de Errores del Sistema](#gestión-de-errores-del-sistema)
  - [7. Comandos de Shell y Utilidades del Entorno](#7-comandos-de-shell-y-utilidades-del-entorno)
    - [Redirecciones y Tuberías (Pipes)](#redirecciones-y-tuberías-pipes)
    - [Comandos Útiles de Procesamiento](#comandos-útiles-de-procesamiento)
    - [Lectura en Bucle con read en Bash](#lectura-en-bucle-con-read-en-bash)
  - [8. Errores Frecuentes y Buenas Prácticas](#8-errores-frecuentes-y-buenas-prácticas)

---

## 1. Compilación con GCC

### Comandos y Sintaxis Básica
* `gcc programa.c`: Compila el código fuente y genera por defecto el binario ejecutable `a.out`.
* `gcc -o programa programa.c`: Compila el código fuente asignando `programa` como nombre del binario generado.
* `./programa`: Ejecuta el binario ubicado en el directorio actual (`.` = directorio actual, `/` = separador; obligatorio porque `.` no está en `$PATH` por seguridad).
* `gcc -c programa.c`: Realiza la compilación sin enlazar y produce el fichero objeto `programa.o`.
* `gcc -fsyntax-only programa.c`: Comprueba exclusivamente la validez sintáctica y advertencias sin generar ningún fichero ni `a.out` en disco.
* **Omitir `-o`:** Si se omite `-o`, GCC genera siempre `a.out`, sobreescribiendo silenciosamente cualquier `a.out` anterior.

### Flags y Opciones del Compilador
* `-Wall`: Activa los avisos (*warnings*) ante construcciones dudosas o posibles bugs en el código (se imprime en la terminal indicando `archivo:línea:col: warning:`).
* **Diferencia Warning vs Error:** Un *warning* produce el binario ejecutable a pesar de la advertencia; un *error* detiene la compilación y no genera ejecutable.
* `-Wextra`: Activa avisos adicionales de buenas prácticas que no están cubiertos por `-Wall`.
* `-g`: Incorpora información y tabla de símbolos en el binario para permitir la depuración paso a paso con GDB.
* `-o <nombre>`: Especifica el nombre del archivo de salida resultante.
* `-I<dir>`: Añade un directorio a la lista de rutas de búsqueda de ficheros de cabecera (`#include`).
* `-L<dir>`: Añade un directorio a la lista de rutas de búsqueda de bibliotecas para el enlazador.
* `-l<libreria>`: Enlaza una biblioteca externa (ejemplo: `-lm` para la biblioteca matemática estándar `libm`).
* `--save-temps`: Conserva en disco todos los ficheros temporales generados durante las etapas intermedias.

### Etapas de la Compilación
* **Preprocesado (`-E`):** Resuelve directivas (`#include`, `#define`) y genera código C expandido (`.i`).
  ```bash
  gcc -E programa.c -o programa.i
  ```
* **Compilación a ensamblador (`-S`):** Traduce el código C a instrucciones ensamblador de la arquitectura (`.s`).
  ```bash
  gcc -S programa.c -o programa.s
  ```
* **Ensamblado (`-c`):** Traduce las instrucciones ensamblador a código máquina en un objeto binario no enlazado (`.o`).
  ```bash
  gcc -c programa.c -o programa.o
  ```
* **Enlazado (*Linker*):** Combina ficheros objeto y bibliotecas para construir el ejecutable final.
  ```bash
  gcc programa.o auxiliar.o -o programa -lm
  ```

---

## 2. Automatización con GNU Make

### Estructura Básica de un Makefile
Un Makefile define reglas que relacionan un objetivo (*target*) con sus dependencias y las órdenes para construirlo.
```makefile
objetivo: dependencias
	receta_con_tabulador_inicial
```

### Variables Convencionales
* `CC`: Variable que almacena el nombre del compilador de C (por defecto `gcc`).
* `CFLAGS`: Opciones pasadas al compilador en la fase de compilación (ejemplo: `-Wall -g`).
* `LDFLAGS`: Parámetros de búsqueda pasados al enlazador (ejemplo: `-L/usr/lib`).
* `LDLIBS` o `LIBS`: Bibliotecas que se deben enlazar en el binario final (ejemplo: `-lm`).
* `$@`: Variable automática que representa el nombre del objetivo de la regla.
* `$<`: Variable automática que representa la primera dependencia de la regla.
* `$^`: Variable automática que representa la lista completa de todas las dependencias.

### Comandos de Make
* `make`: Ejecuta la primera regla definida en el Makefile por defecto.
* `make <objetivo>`: Construye específicamente el objetivo indicado (ejemplo: `make clean`).
* `touch archivo.c`: Actualiza la marca temporal de un archivo para obligar a Make a recompilarlo.

### Objetivos Phony (.PHONY)
* `.PHONY: clean`: Declara que el objetivo `clean` es una orden y no un archivo real en el disco.
* Previene conflictos en caso de que exista un archivo físico llamado `clean` en la misma carpeta.

---

## 3. Depuración con GDB

### Preparación y Lanzamiento
* `gcc -g -o prog prog.c`: Compila incluyendo la tabla de símbolos requerida por el depurador.
* `gdb ./prog`: Inicia el depurador cargando el ejecutable especificado.
* `quit` o `q`: Sale del intérprete interactivo de GDB.

### Puntos de Ruptura (Breakpoints)
* `break main` (o `b main`): Inserta un punto de interrupción al comienzo de la función `main`.
* `break <linea>`: Pausa la ejecución al alcanzar el número de línea indicado en el fichero actual.
* `break archivo.c:<linea>`: Establece un punto de ruptura en una línea de un archivo concreto.
* `info breakpoints`: Muestra la lista de puntos de ruptura activos con sus identificadores.
* `delete <id>`: Elimina el punto de interrupción asociado a ese número identificador.

### Control de Flujo de Ejecución
* `run` (o `r`): Inicia la ejecución del programa desde el principio.
* `run arg1 arg2`: Inicia el programa pasando argumentos de línea de comandos.
* `next` (o `n`): Ejecuta la siguiente instrucción avanzando sin entrar dentro de funciones (*step over*).
* `step` (o `s`): Ejecuta la siguiente instrucción entrando al interior de funciones llamadas (*step into*).
* `continue` (o `c`): Reanuda la ejecución continua hasta el siguiente punto de interrupción o fin.
* `finish`: Ejecuta el resto de la función actual y se detiene justo al retornar de ella.

### Inspección de Datos y Memoria
* `print <var>` (o `p <var>`): Imprime en pantalla el valor actual de una variable o expresión.
* `display <var>`: Muestra automáticamente el valor de la variable en cada paso de ejecución.
* `list` (o `l`): Muestra las líneas del código fuente circundantes al punto actual.
* `x/<formato> <direccion>`: Examina el contenido crudo de memoria en una dirección física (ej: `x/4x ptr`).

### Diagnóstico de Fallos
* `backtrace` (o `bt`): Muestra la traza completa de la pila de llamadas (*stack frame*) al ocurrir un fallo.
* `Ctrl + C`: Pausa inmediatamente la ejecución del programa si entra en un bucle infinito.

### Flujo de Depuración Paso a Paso
1. **Compilar con `-g`:** `gcc -g -o programa programa.c` (indispensable para conservar símbolos y nombres de variables).
2. **Abrir GDB:** `gdb ./programa` (entra en la consola interactiva del depurador).
3. **Poner punto de interrupción:** `break main` (o en una línea concreta con `break 25`).
4. **Arrancar:** `run` (o `run arg1` si el programa espera argumentos).
5. **Avanzar paso a paso:** Usa `next` (`n`) para avanzar de línea sin entrar en funciones, o `step` (`s`) para entrar dentro de ellas.
6. **Inspeccionar:** Usa `print variable` (`p variable`) para comprobar que los valores son los correctos.
7. **Reanudar o detener:** Usa `continue` (`c`) para ir al siguiente breakpoint o `Ctrl + C` si el programa no responde.
8. **En caso de cuelgue o Segmentation Fault:** Escribe `bt` para que GDB te indique la línea exacta y función donde ocurrió el fallo.
9. **Salir de GDB:** `quit` (`q`).

---

## 4. Fundamentos del Lenguaje C

### Tipos de Datos y Operador sizeof
* `sizeof(tipo)`: Operador en tiempo de compilación que devuelve el tamaño en bytes de un tipo o variable.
* `char`: Entero de 1 byte usado habitualmente para almacenar caracteres ASCII (valores de -128 a 127).
* `short`: Entero corto con signo que ocupa típicamente 2 bytes.
* `int`: Entero estándar del sistema que ocupa habitualmente 4 bytes en plataformas de 32 y 64 bits.
* `long`: Entero largo que ocupa 8 bytes en arquitecturas Linux x86_64.
* `float`: Número en coma flotante de precisión simple que ocupa 4 bytes.
* `double`: Número en coma flotante de doble precisión que ocupa 8 bytes.

### Modificadores de Formato en Entrada/Salida
* `%d` o `%i`: Formato para enteros con signo en base decimal (`int`).
* `%u`: Formato para enteros sin signo (`unsigned int`).
* `%lu`: Formato para enteros largos sin signo devueltos habitualmente por `sizeof` (`size_t`).
* `%c`: Formato para leer o escribir un único carácter (`char`).
* `%s`: Formato para cadenas de caracteres terminadas en byte nulo (`char *`).
* `%p`: Formato para direcciones de memoria expresadas en hexadecimal (`void *`).
* `%f`: Formato para números en coma flotante estándar (`float` o `double`).

### Punteros y Operadores de Dirección
* `&variable`: Operador unario que obtiene la dirección de memoria donde reside una variable.
* `*puntero`: Operador de indirección que accede al valor almacenado en la dirección a la que apunta el puntero.
* `tipo *p`: Declaración de una variable `p` como un puntero a una zona de tipo `tipo`.
* `NULL`: Macro que representa una dirección de memoria nula inválida (puntero que no apunta a nada).

### Gestión de Memoria Dinámica
* `malloc(nbytes)`: Reserva un bloque continuo de memoria en el *heap* del tamaño en bytes solicitado sin inicializar.
* `calloc(nelem, size)`: Reserva memoria para un array de elementos y limpia a cero todos sus bytes.
* `realloc(ptr, nuevo_tam)`: Redimensiona un bloque previamente reservado preservando su contenido.
* `free(ptr)`: Libera la memoria previamente asignada en el *heap* para evitar fugas (*memory leaks*).

### Arrays y Decaimiento a Punteros
* Un array decae automáticamente a un puntero que apunta a su primer elemento (`array == &array[0]`).
* `sizeof(array)` dentro de su ámbito original devuelve el tamaño total de la memoria asignada en bytes.
* Al pasar un array como parámetro a una función, este decae a un puntero perdiendo su longitud original.
* Copia de arrays: No es posible mediante `arr1 = arr2`; se debe copiar elemento a elemento o con `memcpy`.

### Cadenas de Caracteres (Strings)
* Una cadena en C es un array de caracteres que concluye obligatoriamente con el byte nulo terminador `'\0'`.
* `char *s = "texto"`: Puntero a una cadena literal de solo lectura situada en el segmento `.rodata`.
* `char s[] = "texto"`: Array local mutable en el *stack* inicializado con los caracteres indicados.
* `strlen(s)`: Devuelve la cantidad de caracteres de la cadena sin incluir el byte terminador nulo.
* `strcpy(dest, orig)`: Copia el contenido de la cadena origen en el búfer de destino.
* `strdup(s)`: Duplica una cadena reservando automáticamente la memoria requerida en el *heap* con `malloc`.

### Estructuras (struct) y typedef
* `struct`: Tipo de dato compuesto que agrupa variables de diferentes tipos bajo un mismo registro.
* `typedef`: Crea un alias de tipo para simplificar la escritura en las declaraciones.
* Operador punto (`.`): Accede a los campos de una estructura cuando se tiene la variable directa.
* Operador flecha (`->`): Accede a los campos de una estructura a través de un puntero a ella (`p->campo`).

### Paso de Parámetros en Funciones
* **Paso por valor:** La función recibe una copia exacta del dato; las modificaciones no afectan a la variable original.
* **Paso por referencia:** Se pasa la dirección de memoria de la variable mediante un puntero para permitir mutarla.

---

## 5. Argumentos de Línea de Comandos y getopt

### argc y argv
* `int argc`: Representa el número total de argumentos pasados al programa desde la línea de órdenes.
* `char *argv[]`: Array de punteros a cadenas donde `argv[0]` es el nombre del programa ejecutable.

### Función getopt
Permite procesar sistemáticamente opciones cortas pasadas por línea de comandos.
```c
int opt = getopt(argc, argv, "heo:l:");
```
* Una letra simple (ejemplo `"h"`): Indica una opción booleana sin argumento obligatorio.
* Una letra seguida de dos puntos (ejemplo `"o:"`): Indica una opción que exige un argumento complementario.

### Variables Globales Asociadas
* `optarg`: Puntero global que contiene la cadena con el argumento asociado a la opción actual procesada.
* `optind`: Índice del siguiente elemento de `argv` pendiente de procesar tras concluir el bucle de opciones.
* `opterr`: Variable entera que si vale cero deshabilita los mensajes automáticos de error de `getopt`.

### Conversión de Cadenas a Números
* `atoi(cadena)`: Convierte una cadena de texto en un entero simple de tipo `int`.
* `strtol(cadena, &fin, base)`: Conversión robusta de cadena a entero largo (`long`) detectando caracteres erróneos.

---

## 6. Entrada/Salida y Manejo de Ficheros

### Flujos Estándar y Apertura
* `stdin`, `stdout`, `stderr`: Descriptores de flujo predefinidos para entrada, salida y errores estándar.
* `fopen("ruta", "modo")`: Abre un fichero en modo lectura (`"r"`), escritura (`"w"`) o añadido (`"a"`).
* `fclose(fichero)`: Cierra el flujo del archivo liberando los búferes y descriptores del sistema operativo.

### Lectura y Escritura
* `fgets(buffer, tam, flujo)`: Lee de manera segura una línea de texto de un flujo hasta un salto de línea.
* `fprintf(flujo, formato, ...)`: Escribe una cadena formateada en un flujo de salida específico o fichero.
* `fscanf(flujo, formato, ...)`: Lee datos formateados a partir de un flujo de entrada.

### Procesamiento con sscanf y strsep
* `sscanf(buffer, formato, ...)`: Parsea y extrae datos con formato a partir de una cadena en memoria.
* `strsep(&puntero, delimitador)`: Divide una cadena en tokens modificando la fuente original sustituyendo delimitadores por `'\0'`.

### Gestión de Errores del Sistema
* `errno`: Variable global del sistema que almacena el código numérico del último error acontecido.
* `perror("mensaje")`: Imprime el mensaje provisto seguido de la descripción textual del error actual de `errno`.

---

## 7. Comandos de Shell y Utilidades del Entorno

### Redirecciones y Tuberías (Pipes)
* `>`: Redirige la salida estándar a un archivo sobreescribiendo su contenido previo.
* `>>`: Redirige la salida estándar a un archivo concatenando los datos al final.
* `<`: Redirige el contenido de un archivo hacia la entrada estándar de un proceso.
* `2>`: Redirige el flujo de errores estándar (`stderr`) a un archivo especificado.
* `|`: Tubería que conecta la salida estándar del comando izquierdo con la entrada estándar del derecho.

### Comandos Útiles de Procesamiento
* `grep "patron" archivo`: Filtra e imprime las líneas de un archivo que coinciden con una expresión.
* `cut -d':' -f1 archivo`: Corta campos específicos de un archivo de texto según un delimitador concreto.
* `cat archivo`: Muestra el contenido completo de uno o varios ficheros en la salida estándar.
* `head -n X / tail -n X`: Muestra las primeras o últimas X líneas de un flujo o archivo de texto.

### Lectura en Bucle con read en Bash
Lectura estructurada registro por registro procesando delimitadores:
```bash
while IFS=':' read -r login pass uid gid name home shell; do
    echo "Usuario: $login con Home: $home"
done < /etc/passwd
```

---

## 8. Errores Frecuentes y Buenas Prácticas

* **Confundir `=` con `==`:** Usar asignación en lugar de comparación dentro de un bloque condicional `if (x = 2)`.
* **Variables sin inicializar:** Emplear una variable local acumuladora antes de otorgarle un valor inicial explícito.
* **Fugas de memoria (*Memory Leaks*):** Solicitar bloques de memoria mediante `malloc` sin liberar con `free`.
* **Uso de memoria tras liberar (*Use-After-Free*):** Intentar acceder o escribir en un puntero que ya fue liberado.
* **Desbordamiento de búfer (*Buffer Overflow*):** Escribir más bytes de los reservados en un array o memoria dinámica.
* **Olvidar el carácter nulo terminador:** Manejar cadenas de caracteres sin reservar espacio para el byte final `'\0'`.
* **Retornar direcciones del stack:** Devolver la dirección de una variable local automática al salir de una función.
