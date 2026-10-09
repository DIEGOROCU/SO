#!/bin/bash

# $1 es el primer argumento posicional. ${1:-/etc/passwd} asigna /etc/passwd si $1 está vacío.
FILE=${1:-/etc/passwd}
entry_number=0

# IFS=':' altera el "Internal Field Separator" solo para este read. 'read -r' previene escapar barras invertidas.
while IFS=':' read -r login_name encrypted_pass uid gid user_name user_home user_shell
do
    # Comprueba si variable está vacía (-z) o es un comentario que empieza por '#' (usando match/strip paramétrico)
    if [ -z "$login_name" ] || [ "${login_name#\#}" != "$login_name" ]; then
        continue # Ignorar línea y pasar a la siguiente
    fi

    # dirname extrae la ruta al directorio superior. $() ejecuta comandos y captura la salida
    if [ "$(dirname "$user_home")" = "/home" ]; then
        # printf ofrece un string format %s tipo C en lugar de echo, es más seguro y fácil de predecir
        printf '[Entry #%d]\n\tlogin=%s\n\tenc_pass=%s\n\tuid=%s\n\tgid=%s\n\tuser_name=%s\n\thome=%s\n\tshell=%s\n' \
            "$entry_number" "$login_name" "$encrypted_pass" "$uid" "$gid" \
            "$user_name" "$user_home" "$user_shell"
        
        # $(( ... )) es un bloque de evaluación aritmética en Bash
        entry_number=$((entry_number + 1))
    fi
# < manda el archivo FILE como Standard Input al bucle while completo
done < "$FILE"
