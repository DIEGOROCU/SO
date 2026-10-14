# Respuestas - Ejercicio 2: Creación y paso de parámetros a hilos

## 1. Funcionamiento de la Solución Correcta (`hilos.c`)

En la solución correcta:
1. **Estructura de argumentos**: Se define la estructura `struct thread_args` con dos campos:
   - `int num`: número identificativo del hilo ($0, 1, \dots, N-1$).
   - `char prioridad`: carácter `'P'` para hilos pares y `'N'` para impares.
2. **Memoria dinámica independiente**: Por cada hilo a crear en el bucle principal, se realiza una reserva con `malloc(sizeof(struct thread_args))`. De este modo, cada hilo recibe un puntero a una región de memoria propia y exclusiva en el Heap.
3. **Rutina del hilo (`thread_usuario`)**:
   - Copia los campos de la estructura a variables locales situadas en su propia pila (Stack).
   - Libera la memoria dinámica asignada a sus argumentos llamando a `free(args)` para evitar fugas de memoria (*memory leaks*).
   - Obtiene su identificador de hilo POSIX mediante `pthread_self()`.
   - Imprime por salida estándar su identificador (TID), el número de hilo y su prioridad.
4. **Sincronización**: El hilo principal invoca `pthread_join()` para cada uno de los hilos creados, asegurando que el proceso principal no termine antes de que todos los hilos hayan finalizado su ejecución.

---

## 2. Cuestión: ¿Qué sucede al usar una sola variable para todos los hilos y cuál es la razón?

> **Enunciado de la prueba:**
> *Probar a crear solamente una variable para el argumento de todos los hilos, dándole el valor correspondiente a cada hilo antes de la llamada a `pthread_create`. Explicar qué sucede y cuál es la razón.*

### Qué sucede (Comportamiento observado):

1. **Valores duplicados y valores perdidos (Condición de carrera / Data Race)**:
   - Muchos hilos imprimen el mismo número de hilo y la misma prioridad (típicamente los últimos valores asignados en el bucle del `main`).
   - Varios identificadores numéricos nunca llegan a imprimirse (por ejemplo, los hilos $0, 1, 2$ pueden no aparecer nunca).
   - Se observan inconsistencias en la relación par-prioridad (por ejemplo, un hilo con número impar como $3$ puede llegar a imprimir prioridad `'P'`, debido a que el hilo principal modificó el campo `num` o `prioridad` a mitad de la lectura del hilo).

2. **Fallo de segmentación o aborto por doble liberación (*Double Free / Invalid Pointer*)**:
   - Si la variable se reservó en memoria dinámica una sola vez y todos los hilos ejecutan `free(args)`: el primer hilo que ejecute `free()` liberará la memoria; los siguientes hilos accederán a memoria ya liberada (*use-after-free*) y al llamar a `free()` provocarán un error en tiempo de ejecución:
     ```text
     free(): double free or corruption (out)
     Aborted (core dumped)
     ```
   - Si la variable se declaró estáticamente en la pila (`struct thread_args arg`), la llamada `free(args)` fallará de inmediato con:
     ```text
     free(): invalid pointer
     Aborted (core dumped)
     ```

### Cuál es la razón técnica:

1. **Memoria compartida y asincronía**:
   Los hilos de un mismo proceso comparten el mismo espacio de direcciones virtuales (código, datos globales y Heap). Al pasarle a cada hilo un puntero a la misma variable (`&arg`), todos los hilos comparten el acceso a la misma dirección de memoria.

2. **Planificación no determinista del sistema operativo**:
   La llamada a `pthread_create()` solicita al sistema operativo la creación del nuevo hilo, pero **no garantiza que el hilo hijo empiece a ejecutarse inmediatamente**.
   Mientras el planificador del sistema operativo asigna tiempo de CPU al nuevo hilo, el hilo principal (`main`) continúa ejecutando el bucle `for`, sobreescribiendo el contenido de dicha variable compartida con los datos de las siguientes iteraciones.

3. **Falta de sincronización**:
   Dado que no existe ningún mecanismo de exclusión mutua o sincronización entre el hilo principal que escribe en la variable y los hilos hijos que leen de ella, se produce una **condición de carrera (*race condition*)**: el valor que leerá un hilo depende exclusivamente de la velocidad relativa a la que se planifiquen el hilo principal y el hilo hijo.

### Conclusión y Buena Práctica

Para pasar parámetros de manera segura a hilos en POSIX sin introducir carreras:
- O bien se asigna una **estructura independiente en memoria dinámica (`malloc`)** para cada hilo y el hilo receptor la libera (`free`), como se ha implementado en `hilos.c`.
- O bien se utiliza un **array de estructuras** (ej. `struct thread_args args[NUM_HILOS]`) indexado por el número de hilo, de modo que cada hilo apunte a una posición de memoria distinta que no se sobreescriba.
