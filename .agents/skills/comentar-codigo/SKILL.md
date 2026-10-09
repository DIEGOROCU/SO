---
name: comentar-codigo
description: >-
  Usa esta skill cuando el usuario te pida comentar código, añadir explicaciones a los ficheros, o revisar prácticas de C y Bash. Instruye sobre cómo crear comentarios "one-liner" didácticos.
---

# Skill: Comentar Código (Prácticas SO)

Esta skill define el estándar para comentar código en los ejercicios de las prácticas (especialmente en C y Bash).

## Directrices para comentar código

1. **Brevedad y Claridad (One-liners)**: Los comentarios deben ser breves, directos y preferiblemente de una sola línea. No satures el código con párrafos largos.
2. **Enfoque Didáctico**: El objetivo es que el estudiante entienda el *por qué* y no solo el *qué*. 
3. **Cosas a priorizar en los comentarios**:
   - **Sintaxis peculiar**: Explica funciones que mutan variables por referencia (ej. `strsep`, `getopt`), decaimiento de arrays, aritmética de punteros o uso de macros (preprocesador).
   - **Flags y Syscalls (POSIX vs stdio)**: Explica los parámetros y flags de llamadas al sistema (ej. `O_TRUNC`, `O_CREAT` y su permiso `0666`). Destaca las diferencias entre flujos `FILE*` (ej. `fopen`, `fread`) y File Descriptors enteros (ej. `open`, `read`, `write`).
   - **Gestión de Memoria**: Aclara cuándo un dato se aloja en el Heap (`malloc`, `strdup`) y requiere hacer un `free()` para evitar fugas (memory leaks), frente a reservas estáticas en el Stack (pila).
   - **Particularidades en Bash**: Explica manipulaciones y expansiones de cadenas (ej. `${var#\#}`), la variable `IFS`, redirecciones de entrada (`done < $FILE`), y evaluación aritmética (`$(( ))`).
   - **Códigos de retorno**: Aclara qué devuelven las funciones en caso de éxito/error (ej. `-1` indicando error de syscall seteando `errno`, frente a `NULL` o `0`).
4. **No redundancia**: No comentes cosas triviales o literales como `int i = 0; // asigna cero a i`. Comenta el propósito subyacente (ej. `// Inicializar a cero para no arrastrar basura de la pila`).

## Procedimiento de Ejecución

Cuando el usuario pida añadir comentarios a un código:
1. Analiza el fichero completo.
2. Identifica los puntos críticos mencionados arriba.
3. Inserta los comentarios directamente encima de la línea o bloque correspondiente.
4. Mantén la convención del lenguaje (`//` para C moderno, `/* */` si el contexto lo requiere, `#` para Bash).
5. Usa herramientas de sobreescritura seguras en lugar de hacer el reemplazo manual por strings si hay múltiples ficheros.
