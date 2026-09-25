# Ejercicio 5

En este ejercicio se practica la programacion en Bash usando la orden interna `read` (consulta `help read`) para procesar ficheros linea a linea.

`read` lee una linea de la entrada estandar, la descompone en palabras y asigna cada palabra a una variable. Para usar un delimitador especial se puede asignar un valor a `IFS` antes de ejecutar `read`. Por ejemplo:

```bash
while IFS=':' read var1 var2 ...; do
    # procesar var1, var2, ...
done < fichero
```

El fichero `/etc/passwd` contiene siete campos separados por `:`: nombre de login, contrasena cifrada, UID, GID, nombre descriptivo, directorio personal y shell.

## Tareas

Utiliza `read` para crear un script que haga lo mismo que el programa anterior `show-passwd` con sus opciones por defecto:

* Lee `/etc/passwd`.
* Parsea sus entradas, formadas por lineas con campos separados por `:`.
* Muestra cada entrada por la salida estandar con el mismo formato verbose que `show-passwd`:

```text
[Entry #n]
        login=...
        enc_pass=...
        uid=...
        gid=...
        user_name=...
        home=...
        shell=...
```

El contador `n` empieza en cero. Las lineas que comienzan por `#` no representan entradas y deben ignorarse.

Para obtener salida con formato en Bash se puede consultar la opcion `-e` de `echo` (`man echo`). Alternativamente, puede usarse `printf` (`man 1 printf`).

Una vez hecho esto, modifica el script para que solo se muestren aquellas entradas de `/etc/passwd` cuyo `home` sea un subdirectorio directo de `/home`, es decir, aquellas para las que `dirname home` sea exactamente `/home`. Para ello resultaran utiles `dirname` (`man dirname`) y la estructura `if` junto con `test` o `[`. Mantén el mismo formato de salida.

Finalmente, obten una orden Bash que combine `cut` y `grep` para mostrar del fichero `/etc/passwd` todos los homes que empiecen por `/home`. Consulta las paginas de manual de `cut` y `grep` y revisa el uso de pipes (`|`) para combinar comandos del shell.
