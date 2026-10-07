# Ejercicio 6: Permisos y modos de apertura

En segundo lugar, trabajaremos con la modificación de permisos de acceso a un determinado fichero. Para ello, consulta la página de manual de la orden chmod, y observa los dos modos soportados para especificar los permisos que se desean otorgar a un determinado fichero:
- Modo simbólico, que usa un argumento mnemotécnico para especificar los permisos a otorgar a un determinado fichero.
- Modo octal, que utiliza un número en base octal para representar el patrón de bits del campo mode del i-nodo asociado al fichero.

Experimenta con la orden chmod y otorga a cualquier fichero permisos de lectura, escritura o ejecución usando sus dos modos de funcionamiento. Observa las implicaciones del cambio de modo a la hora de leer el contenido del fichero (cat) o escribir en él (a través, por ejemplo de echo), así como los cambios que se producen en el i-nodo correspondiente (stat o ls -l).

Por último, es importante diferenciar entre los permisos otorgados a un determinado fichero o directorio (estáticos y almacenados en su i-nodo asociado) y el modo de apertura del mismo desde un programa escrito en C (dinámico y almacenado en las tablas internas del sistema operativo), que restringen, en última instancia, las operaciones que se podrán realizar sobre el fichero desde el proceso en ejecución.
Para ello, escribe un programa en C llamado apertura.c (compilado como apertura.x) que, utilizando llamadas al sistema POSIX, reúna las siguientes características:
1. El programa recibirá un argumento obligatorio (-f) seguido del nombre del fichero a abrir.
2. El programa recibirá, a través de dos argumentos opcionales (-r para lectura y -w para escritura) el modo de apertura deseado para el fichero. Es necesario proporcionar al menos un modo de apertura, y ambos pueden combinarse para que la apertura sea en modo lectura/escritura.
3. El programa intentará abrir (open) el fichero con el modo indicado, reportando error si no es posible. Si el fichero no existe, se creará. En caso contrario, se eliminará todo su contenido.
4. En todo caso, e independientemente del modo de apertura seleccionado, el programa intentará realizar una escritura (write) desde el fichero y a continuación una lectura (read), reportando un error si no es posible realizar alguna de estas operaciones.
5. Por último, se cerrará el fichero (close).

Una vez desarrollado, experimenta al menos con las siguientes situaciones:
1. Otorga permisos de lectura y escritura a un fichero existente en el sistema, y ejecuta el programa apertura con las tres opciones disponibles (-r, -w y -rw). ¿Es posible leer y/o escribir en el fichero en todos los casos? ¿Qué función devuelve en este caso el error? ¿Por qué?
2. Elimina el permiso de lectura sobre el fichero destino. ¿Qué función devuelve en este caso el mensaje de error? ¿Por qué? (puedes también hacer lo propio eliminando el permiso de escritura, o los dos simultáneamente).
3. Elimina el permiso de ejecución del fichero ejecutable apertura.x. ¿Qué implicación tiene esto de cara a la ejecución del mismo (./apertura.x).
