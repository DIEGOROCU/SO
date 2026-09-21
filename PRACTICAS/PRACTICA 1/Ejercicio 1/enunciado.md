En el directorio ejercicio1 de los ficheros para la práctica ( ficheros_p1.tar.gz) hay una serie de subdirectorios con códigos de pequeños programas de C que pretenden poner de manifiesto algunos de los errores frecuentes que cometen los programadores con poca experiencia con C así como familiarizar al estudiante con las herramientas básicas de compilación que se usan en un entorno Linux.
Para cada directorio se proporciona una serie de tareas y preguntas que deberás responder, para las cuales tendrás que examinar y probar los ejemplos proporcionados. Consulta el manual del entorno para saber como utilizar el compilador. Puedes utilizar VSCode como editor.

1. Compilación
• Compila el código del ejercicio y ejecútalo
• Obtén la salida de la etapa de pre-procesado (opción -E o la opción --save-temps para obtener la salida de todas las etapas intermedias) y en un fichero hello2.i
• ¿Qué ha ocurrido con la “llamada a min()” en hello2.i?
• ¿Qué efecto ha tenido la directiva #include <stdio.h>?

2. Herramienta make
• Examina el makefile, identifica las variables definidas, los objetivos (targets) y las regalas.
• Ejecuta make en la linea de comandos y comprueba las ordenes que ejecuta para construir el proyecto.
• Marca el fichero aux.c como modificado ejecutando touch aux.c. Después ejecuta de nuevo make. ¿Qué diferencia hay con la primera vez que lo ejecutaste? ¿Por qué?
• Ejecuta la orden make clean. ¿Qué ha sucedido? Observa que el objetivo clean está marcado como phony en la directiva .PHONY: clean. ¿por qué? Para comprobarlo puedes comentar dicha línea del makefile, compilar de nuevo haciendo make, y después crear un fichero en el mismo directorio que se llame clean, usando el comando touch clean. Ejecuta ahora make clean, ¿qué pasa?
• Comenta la línea LIBS = -lm poniendo delante una almoadilla (#). Vuelve a construir el proyecto ejecutando make (haz un clean antes si es necesario). ¿Qué sucede? ¿Qué etapa es la que da problemas?

3. Tamaño de variables
Compila y ejecuta el código de cada uno de los ejemplos proporcionados y responde a las preguntas proporcionadas para ellos.
• main1.c
– ¿Por qué el primer printf() imprime valores distintos para ‘a’ con los modificadores %d y %c?
– ¿Cuánto ocupa un tipo de datos char?
– ¿Por qué el valor de ‘a’ cambia tanto al incrementarlo en 6? (la respuesta está relacionada con la cuestión anterior)
– Si un “long” y un “double” ocupan lo mismo, ¿por qué hay 2 tipos de datos diferentes?
• main2.c
– ¿Tenemos un problema de compilación o de ejecución?
– ¿Por qué se da el problema?. Soluciónalo, compila y ejecuta de nuevo.
– ¿Qué significa el modificar "%lu" en printf()?
– ¿A qué dirección apunta "pc"? ¿Coincide con la de alguna variable anteriormente declarada? Si es así, ¿Coinciden los tamaños de ambas?
– ¿Coincide el valor del tamaño de array1 con el número de elementos del array? ¿Por qué?
– ¿Coinciden las direcciones a la que apunta str1 con la de str2?
– ¿Por qué los tamaños (según sizeof()) de str1 y str2 son diferentes?

4. Arrays
Compila y ejecuta el código de los ejemplos proporcionados y responde a las preguntas propuestas para cada uno de ellos.
• array1.c
– ¿Por qué no es necesario escribir "&list" para obtener la dirección del array list?
– ¿Qué hay almacenado en la dirección de list?
– ¿Por qué es necesario pasar como argumento el tamaño del array en la función init_array?
– ¿Por qué el tamaño devuelto por sizeof() para el array de la función init_array no coincide con el declarado en main()?
– ¿Por qué NO es necesario pasar como argumento el tamaño del array en la función init_array2?
– ¿Coincide el tamaño devuelto por sizeof() para el array de la función init_array2 con el declarado en main()?
• array2.c
– ¿La copia del array se realiza correctamente? ¿Por qué?
– Si no es correcto, escribe un código que sí realice la copia correctamente.

5. Punteros
Compila y ejecuta el código de los ejemplos y responde a las cuestiones proporcionadas para cada uno de ellos.
• punteros1.c
– ¿Qué operador utilizamos para declarar una variable como un puntero a otro tipo?
– ¿Qué operador utilizamos para obtener la dirección de una variable?
– ¿Qué operador se utiliza para acceder al contenido de la dirección “a la que apunta” un puntero?
– Hay un error en el código. ¿Se produce en compilación o en ejecución? ¿Por qué se produce?
• punteros2.c
– ¿Cuántos bytes se reservan en memoria con la llamada a malloc()?
– ¿Cuál es la dirección del primer y último byte de dicha zona reservada?
– ¿Por qué el contenido de la dirección apuntada por ptr es 7 y no 5 en el primer printf()?
– ¿Por qué se modfica el contenido de ptr[1] tras la sentencia *ptr2=15;?
– Indica dos modos diferentes de escribir el valor 13 en la dirección correspondiente a ptr[100].
– Hay un error en el código. ¿Se manifiesta en compilación o en ejecución? Aunque no se manifieste, el error está. ¿Cuál es?
• punteros3.c
– ¿Por qué cambia el valor de ptr[13] tras la asignación ptr = &c;?
– El código tiene (al menos) un error. ¿Se manifiesta en compilación o en ejecución? ¿Por qué?
– ¿Qué ocurre con la zona reservada por malloc() tras a asignación ptr = &c;? ¿Cómo se puede acceder a ella? ¿Cómo se puede liberar dicha zona?

6. Funciones
Compila y ejecuta el código de cada uno de los ejemplos proporcionados y responde a las cuestiones proporcionadas para cada uno de ellos.
• arg1.c
– ¿Por qué el valor de xc no se modifica tras la llamada a sumC? ¿Dónde se modifica esa información?
– Comenta las dos declaraciones adelantadas de sum() y sumC(). Compila de nuevo, ¿Qué ocurre?
• arg2.c
– ¿Por qué cambia el valor de y tras la llamada a sum()?
– ¿Por qué en ocasiones se usa el operador ‘.’ y en otras ‘->’ para acceder a los campos de una estructura?
– ¿Por qué el valor de zc pasa a ser incorrecto sin volver a usarlo en el código?
– Corrije el código para evitar el error producido en zc

7. Cadenas de caracteres (strings)
Compila y ejecuta el código de cada uno de los ejemplos proporcionados y responde a las cuestiones proporcionadas para cada uno de ellos.
• strings1.c
– El código contiene un error. ¿Se manifiesta en compilación o en ejecución? ¿Por qué se produce? Soluciona el error comentando la(s) línea(s) afectadas. Vuelve a compilar y ejecutar.
– ¿En qué dirección está la letra 'B' de la cadena "Bonjour"? ¿Y la de la la letra 'j'?
– Tras la asignación p=msg2;, ¿cómo podemos recuperar la dirección de la cadena "Bonjour"?
– ¿Por qué la longitud de las cadenas p y msg2 es 2 tras la línea 30? Se asignan 3 bytes a 'p' que modifican a ambos, pero luego la longitud es sólo 2.
– ¿Por qué strlen() devuelve un valor diferente a sizeof()?
• strings2.c
– El código de copy no funciona. ¿Por qué?
– Usa ahora la función copy2() (descomenta la línea correspondiente). ¿Funciona la copia?
– Propón una implementación correcta de la copia.
– ¿Qué hace la función mod()? ¿Por qué funciona?
