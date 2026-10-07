# Ejercicio 5: Administración de ficheros y directorios

En los dos últimos ejercicios, trabajaremos con los atributos típicos de ficheros, aprendiendo a consultarlos y modificarlos desde línea de comandos, y observando sus implicaciones al interactuar con programas escritos en lenguaje C.
Para ello, se desarrollará un script llamado prepara_ficheros.sh, que realizará las siguientes acciones preliminares:
1. Creará y accederá a un directorio proporcionado como único argumento al script. En caso de no existir, dicho directorio se creará (mkdir). En caso de existir, se borrará todo su contenido usando las opciones adecuadas del comando rm o mediante el comando rmdir.
2. Creará en el directorio especificado un conjunto de ficheros con las siguientes características y nombres:
Nombre Tipo Observaciones Comandos a consultar (man)
subdir Directorio – mkdir
fichero1 Fichero regular Se creará sin contenido touch
fichero2 Fichero regular Se creará y escribirán en él 10 caracteres echo
enlaceS Enlace simbólico Enlace simbólico al fichero fichero2 ln
enlaceH Enlace duro Enlace duro al fichero fichero2 ln

3. Recorrerá todos los ficheros creados, mostrando por pantalla todos sus atributos utilizando la orden stat, de la que puede obtenerse más información consultando la página de manual correspondiente (man 1 stat).

A tenor de los resultados observados tras la ejecución del script, responde razonadamente a las siguientes cuestiones:
1. ¿Cuántos bloques de disco ocupa el fichero fichero1? ¿Y el fichero fichero2? ¿Cuál es su tamaño en bytes?
2. ¿Cuál es el tamaño reportado para subdir? ¿Por qué el campo número de enlaces (Links) en este caso es 2?
3. ¿Comparten número de i-nodo alguno de los ficheros o directorios creados? Incluye en tu respuesta el comportamiento ante un directorio, específicamente de las entradas ocultas del directorio subdir (puedes usar para ello stat, o la combinación de modificadores (-i y -a de ls).
4. Muestra el contenido de enlaceH y de enlaceS utilizando el comando cat. A continuación, borra el fichero fichero2 y repite el procedimiento. ¿Puedes acceder en ambos casos al contenido del fichero?
5. Utiliza la orden touch para modificar las fechas de acceso y modificación del fichero enlaceH. ¿Qué cambios se observan en la salida de stat tras su ejecución? Investiga a través de la página de manual para modificar únicamente una de dichas fechas (modificación o acceso).
