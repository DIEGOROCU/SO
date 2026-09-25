# Respuestas - Ejercicio 5

**Orden bash combinando comandos `cut` y `grep`:**
Para obtener del fichero `/etc/passwd` todos los homes que empiecen por `/home`, se pueden encadenar los comandos utilizando un pipe (`|`). 

La orden completa es:
```bash
cut -d':' -f6 /etc/passwd | grep '^/home'
```

**Explicación:**
*   `cut -d':' -f6 /etc/passwd`: `cut` extrae columnas de un archivo. Con `-d':'` le decimos que el delimitador es el carácter de dos puntos. Con `-f6` le pedimos que se quede solo con el campo 6, que corresponde al "home" del usuario en la estructura de `/etc/passwd`.
*   `|` (pipe): Pasa la salida del comando anterior como entrada al siguiente.
*   `grep "^/home"`: Filtra las líneas que le llegan, quedándose solo con aquellas que empiecen (indicado por el símbolo `^`) por el texto `/home`.
