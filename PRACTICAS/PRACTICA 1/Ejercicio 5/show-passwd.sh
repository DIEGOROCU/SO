#!/bin/bash

FILE=${1:-/etc/passwd}
entry_number=0

while IFS=':' read -r login_name encrypted_pass uid gid user_name user_home user_shell
do
    # El programa de referencia descarta las lineas de comentario.
    if [ -z "$login_name" ] || [ "${login_name#\#}" != "$login_name" ]; then
        continue
    fi

    if [ "$(dirname "$user_home")" = "/home" ]; then
        printf '[Entry #%d]\n\tlogin=%s\n\tenc_pass=%s\n\tuid=%s\n\tgid=%s\n\tuser_name=%s\n\thome=%s\n\tshell=%s\n' \
            "$entry_number" "$login_name" "$encrypted_pass" "$uid" "$gid" \
            "$user_name" "$user_home" "$user_shell"
        entry_number=$((entry_number + 1))
    fi
done < "$FILE"
