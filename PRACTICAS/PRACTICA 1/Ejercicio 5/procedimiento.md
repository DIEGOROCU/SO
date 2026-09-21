# Procedimiento - Ejercicio 5

1.  **Lectura con read e IFS:**
    *   Se establece `IFS=':'` (Internal Field Separator) temporalmente en la condición del bucle `while` para que `read` parta cada línea usando los dos puntos como delimitador.
    *   Se le pasan a `read` todas las variables equivalentes a los campos de `/etc/passwd`: `login_name password uid gid user_name user_home user_shell`.
    *   El bucle recibe datos redirigiendo el archivo al final con `< "$FILE"`.

2.  **Filtrado de Subdirectorios:**
    *   Dentro del bucle `while`, se emplea `dirname "$user_home"` para obtener la ruta "padre" de la variable `$user_home`.
    *   Se usa un bloque condicional `if [ "$(dirname "$user_home")" = "/home" ]; then` para evaluar de manera estricta si el "home" del usuario "cuelga" directamente del directorio `/home`.

3.  **Ejecución:**
    ```bash
    chmod +x show-passwd.sh
    ./show-passwd.sh
    # O para probar con un fichero diferente:
    ./show-passwd.sh mi_fichero_passwd_falso
    ```
