# Procedimiento - Ejercicio 4

1.  **Sustitución de funciones de clonado:**
    *   Comentar la definición e invocaciones de `clone_string()`.
    *   Sustituir dichas llamadas por `strdup(token)`. `strdup` realiza un `malloc` con la longitud del string más 1, y un `strcpy` internamente.

2.  **Añadir las opciones con `getopt()`:**
    *   Configurar `getopt` para que acepte `i:` y `c`.
    *   El caso `i` almacenará `optarg` en una variable `input_file` (que por defecto vale `/etc/passwd`).
    *   El caso `c` encenderá un "flag" (variable entera en 1) de modo CSV.

3.  **Lógica de impresión:**
    *   La función de impresión evaluará el modo CSV. Si está activo, imprimirá las variables usando el formato `%s,%s,%d...`, separando con comas en lugar de los saltos de línea estándar.

4.  **Verificación y Compilación:**
    ```bash
    # Para probar sin tener /etc/passwd en windows, creamos un dummy:
    echo "root:x:0:0:root:/root:/bin/bash" > passwd_dummy
    
    # Compilamos y ejecutamos
    gcc -Wall -o show-passwd show-passwd.c
    ./show-passwd -i passwd_dummy
    ./show-passwd -i passwd_dummy -c
    ```
