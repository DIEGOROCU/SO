#!/bin/bash

# Este script lee /etc/passwd (o el archivo pasado como primer argumento)
# y muestra la información de aquellos usuarios cuyo directorio home
# sea un subdirectorio directo de /home.

if [ "$1" != "" ]; then
    FILE="$1"
else
    FILE="/etc/passwd"
fi

while IFS=':' read -r login_name password uid gid user_name user_home user_shell
do
    # Usamos dirname para obtener el directorio padre del home
    # y test ([ ]) para comprobar si es igual a /home
    if [ "$(dirname "$user_home")" = "/home" ]; then
        echo "Login: $login_name"
        echo "Password: $password"
        echo "UID: $uid"
        echo "GID: $gid"
        echo "Name: $user_name"
        echo "Home: $user_home"
        echo "Shell: $user_shell"
        echo "----------------------"
    fi
done < "$FILE"
