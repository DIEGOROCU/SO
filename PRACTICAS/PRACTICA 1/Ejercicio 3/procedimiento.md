# Procedimiento - Ejercicio 3

1.  **Programación:**
    *   Completar el bucle `while ((opt = getopt(argc, argv, "hel:")) != -1)` asegurando que se procesan correctamente las opciones `-h`, `-e` y `-l`.
    *   Usar `strtol` sobre `optarg` para convertir el string del argumento `-l` a número entero para la longitud.
    *   Una vez agotadas las opciones de `getopt`, usar `optind` sobre `argv` para rescatar el argumento obligatorio posicional que corresponde a `title`.
    *   Desarrollar la lógica de impresión usando un bucle con variables iniciadas en `1` (impares) o `2` (pares) según la "flag" registrada en el switch-case.

2.  **Compilación y Ejecución:**
    ```bash
    # Compilar código
    gcc -Wall -o getopt getopt.c
    
    # Pruebas de funcionamiento
    ./getopt -h
    ./getopt MiTituloSecuencia
    ./getopt -e MiTituloPares
    ./getopt -l 5 -e MiTituloParesCorto
    ```
