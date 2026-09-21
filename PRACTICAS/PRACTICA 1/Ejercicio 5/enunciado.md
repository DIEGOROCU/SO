En este ejercicio vamos a practicar la programación en bash que haga uso de la orden interna read (consulta help read) para procesar ficheros línea a línea:

Este comando lee una línea de la entrada estándar, la descompone en palabras, y asigna la primera palabra a la primera variable de la lista de nombres, la segunda a la segunda variable y así sucesivamente.
Si queremos usar un delimitador especial para separar palabras podemos hacerlo asignando valor a la variable IFS antes de usar la operación read. [...]
Utilizar read para crear un pequeño script que haga lo mismo que el programa anterior show-passwd (con sus opciones por defecto), es decir:
• lea el fichero /etc/passwd
• parsee sus entradas formadas por líneas con palabras separadas por ‘:’
• muestre cada entrada por la salida estándar con el mismo formato que el programa show-passwd.
Para obtener salida con formato en bash consultar la opción -e de echo (man echo). Alternativamente puede usarse la utilidad printf (man 1 printf).

Una vez hecho esto, modificar el script para que sólo se muestren aquellas entradas del fichero /etc/passwd en las que el home del usuario sea un subdirectorio de /home. Para ello resultará muy útil el uso del comando dirname (man dirname) y el uso de la estructura de control de flujo if junto con el programa test o el programa [.

Finalmente, intenta obtener una orden bash, combinando los comandos cut y grep, que permita obtener del fichero /etc/passwd todos los homes que empiecen por /home. Consulta las páginas de manual de cut y grep y revisa el uso de pipes (‘|’) para combinar comandos del shell.
