# Procedimiento - Ejercicio 2

Para llevar a cabo la corrección del programa usando el depurador de GNU (GDB) como se pide en el enunciado, se deben seguir estos pasos:

1.  **Compilar el programa con información de depuración:**
    ```bash
    gcc -g -w -o primes primes_original.c
    ```
    El flag `-g` es esencial ya que incluye la información para que GDB pueda relacionar las instrucciones máquina con el código fuente en C.

2.  **Iniciar GDB:**
    ```bash
    gdb ./primes
    ```

3.  **Depuración (comandos útiles en GDB):**
    *   `break main`: Pone un punto de ruptura al inicio del programa.
    *   `run`: Inicia la ejecución.
    *   `next` (o `n`): Ejecuta la siguiente línea de código saltando las funciones.
    *   `step` (o `s`): Ejecuta la siguiente línea pero entrando dentro de las funciones llamadas.
    *   `print var` (o `p var`): Muestra el valor de una variable.
    *   `continue` (o `c`): Continúa la ejecución hasta el siguiente breakpoint o hasta el final.

Al ir paso por paso ejecutando el código original y analizando los valores con `print`, uno detecta cosas como bucles infinitos en `compute_primes`, variables sin inicializar en `sum`, y validaciones condicionales incorrectas (`=` en lugar de `==`) en el `main`. Todo ello se ha subsanado en el archivo `primes_fixed.c`.
