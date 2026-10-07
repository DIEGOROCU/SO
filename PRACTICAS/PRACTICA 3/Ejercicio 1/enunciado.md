# Ejercicio 1: Copia de ficheros regulares

Diseña un programa copy.c que permita hacer la copia de un fichero regular usando las llamadas al sistema del estándar POSIX: open, read, write y close. Se deben consultar sus páginas de manual, prestando especial atención a los flags de apertura: O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC.

El programa recibira dos parámetros por la línea de llamadas. El primero será el nombre del fichero a copiar (fichero origen) y el segundo será el nombre que queremos darle a la copia (fichero destino).

El programa debe realizar la copia en bloques de 512B, usando un array local como almacenamiento intermedio entre la lectura y la escritura. El programa debe ir leyendo bloques de 512 bytes del fichero origen y escribiendo los bytes leídos en el fichero destino. Debe tenerse en cuenta que si el tamaño del fichero no es múltiplo de 512 bytes la última vez no se leerán 512 bytes, sino lo que quede hasta el final del fichero (conultar el apartado RETURN VALUE en la página de manual de read). Por ello siempre se deben escribir en el fichero destino tantos bytes como se hayan leído del fichero origen.

Para comprobar el efecto de O_TRUNC, se sugiere al alumno que antes de ejecutar su programa de copia, cree un fichero con cualquier contenido que se llame como el fichero destino. Después puede copiar otro fichero usando el nombre elegido para el fichero destino y comprobar que el contenido anterior desaparece al usarse el flag O_TRUNC.

Para comprobar el funcionamiento correcto de nuestro programa podemos usar los comandos de shell diff y hexdump (este último para ficheros binarios).
