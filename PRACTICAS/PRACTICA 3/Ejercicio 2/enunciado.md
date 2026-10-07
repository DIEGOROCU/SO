# Ejercicio 2: Enlaces simbólicos

Lo primero que vamos a hacer en este ejercicio es crear un enlace simbólico a un fichero cualquiera usando el comando ln. Por ejemplo, si queremos crear un enlace que se llame mylink y que apunte al fichero ../ejercicio1/Makefile usaremos el siguiente comando del shell:
$ ln -s ../ejercicio1/Makefile mylink

Invocando ls -l podremos comprobar que el fichero creado es realmente un enlace simbólico y veremos el fichero apuntado:
$ ls -l
...
lrwxrwxrwx 1 christian christian 22 Jul 14 13:23 mylink -> ../ejercicio1/Makefile
...

Ahora usaremos nuestro programa de copia para copiar el enlace simbólico. Asumiendo que dicho programa es ../ejercicio1/copy, ejecutamos:
$ ../ejercicio1/copy mylink mylinkcopy

¿Qué tipo de fichero es mylinkcopy? ¿Cuál es el contenido del fichero mylinkcopy? Se pueden usar los comandos ls, stat, cat y diff para obtener las respuestas a estas preguntas.

Es posible que este sea el comportamiento que deseemos, pero también es posible que no. ¿Y si queremos que la copia de un enlace simbólico sea otro enlace simbólico que apunte al mismo fichero que apuntaba el enlace simbólico original?

Vamos a hacer una modificación de nuestro programa de copia del ejercicio anterior, que llamaremos copy2.c. Podemos empezar copiando el programa anterior para luego modificarlo. Haremos entonces una copia usando el comando cp:
$ cp ../ejercicio1/copy.c copy2.c

Después editaremos el fichero copy2.c de modo que: 
1. Antes de hacer la copia identifique si el fichero origen es un fichero regular, un enlace simbólico u otro tipo de fichero, haciendo uso de la llamada al sistema lstat (consultar su página de manual).
2. Si el fichero origen es un fichero regular, haremos la copia como en el ejercicio anterior.
3. En cambio, si el fichero origen es un enlace simbólico no tenemos que hacer la copia del fichero apuntado sino crear un enlace simbólico que apunte al mismo fichero al que apunta el fichero origen. Para ello tenemos que seguir los siguientes pasos:
    a. Reservar memoria para hacer una copia de la ruta apuntada. Una llamada a lstat sobre el fichero origen nos permitirá conocer el número de bytes que ocupa el enlace simbólico, que se corresponde con el tamaño de esta ruta sin el carácter null ('\0') de final de cadena (consultar la página de manual de lstat). Por tanto sumaremos uno al tamaño obtenido de lstat.
    b. Copiar en este buffer la ruta del fichero apuntado haciendo uso de la llamada al sistema readlink. Deberemos añadir manualmente el caracter null de final de cadena.
    c. Usar la llamada al sistema symlink para crear un nuevo enlace simbólico que apunte a esta ruta.

Debéis consultar las páginas de manual de lstat, readlink y symlink.

4. Si el fichero origen es de cualquier otro tipo (por ejemplo un directorio) mostrarán un mensaje de error y el programa terminará.
