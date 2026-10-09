# Respuestas del Ejercicio 5: Administración de ficheros y directorios

A continuación se presentan las respuestas razonadas a las cuestiones planteadas en el ejercicio 5 tras la ejecución y experimentación con el script `prepara_ficheros.sh`.

---

### 1. ¿Cuántos bloques de disco ocupa el fichero `fichero1`? ¿Y el fichero `fichero2`? ¿Cuál es su tamaño en bytes?

#### **Fichero `fichero1`:**
- **Tamaño en bytes:** **0 bytes** (`Size: 0`).
- **Bloques de disco ocupados:** **0 bloques** (`Blocks: 0`).
- **Explicación razonada:**
  El fichero regular `fichero1` fue creado con la orden `touch` sin ningún contenido. Al no contener datos, el sistema de ficheros únicamente crea la entrada de directorio y reserva una estructura de nodo-i (*inode*), pero no asigna ningún bloque de datos en disco.

#### **Fichero `fichero2`:**
- **Tamaño en bytes:** **10 bytes** (`Size: 10`).
- **Bloques de disco ocupados:** **8 bloques** (`Blocks: 8`).
- **Explicación razonada:**
  El fichero `fichero2` fue creado escribiendo la cadena `"0123456789"` mediante `echo -n`, sumando exactamente 10 bytes de contenido.
  El número de bloques reportado por el comando `stat` (correspondiente al campo `st_blocks` de la llamada al sistema `stat(2)` según el estándar POSIX) se contabiliza en unidades de **bloques de 512 bytes** (sectores UNIX tradicionales).
  Por tanto:
  $$\text{Espacio asignado en disco} = 8 \text{ bloques} \times 512 \text{ bytes/bloque} = 4096 \text{ bytes} \ (4\text{ KiB})$$
  Dado que el sistema de ficheros asigna almacenamiento en unidades mínimas de bloques de disco/sistema de archivos de 4096 bytes (`IO Block: 4096`), cualquier fichero con datos (aunque solo contenga 10 bytes) requiere la reserva de al menos un bloque completo del sistema de ficheros (4 KiB), lo que equivale a los 8 bloques de 512 bytes reportados por `stat`.

---

### 2. ¿Cuál es el tamaño reportado para `subdir`? ¿Por qué el campo número de enlaces (Links) en este caso es 2?

- **Tamaño reportado:** **4096 bytes** (`Size: 4096`).
  - **Explicación:** En los sistemas de ficheros UNIX/Linux, un directorio es en realidad un fichero especial cuyo contenido es una lista o tabla de correspondencias entre nombres de elementos y sus respectivos números de nodo-i. Como unidad mínima de asignación, el sistema de ficheros le reserva un bloque completo (4096 bytes en ext4) para almacenar las entradas iniciales que lo componen (específicamente las entradas `.` y `..`).

- **¿Por qué el campo número de enlaces (`Links`) es 2?**
  - El campo `Links` (`st_nlink`) indica cuántas entradas de directorio apuntan hacia el nodo-i en cuestión.
  - Para un directorio recién creado como `subdir`, existen exactamente **2 enlaces duros** que apuntan a su nodo-i:
    1. La entrada con su nombre dentro del directorio padre: `<directorio_padre>/subdir`.
    2. La entrada especial de punto (`.`) dentro del propio `subdir`: `<directorio_padre>/subdir/.`, la cual hace referencia a sí mismo.
  *(Nota adicional: Si en el futuro se crea un subdirectorio hijo dentro de `subdir`, dicho subdirectorio contendrá una entrada `..` que apuntará a `subdir`, incrementando su contador de enlaces a 3. En general, para cualquier directorio: $\text{Links} = 2 + \text{número de subdirectorios hijos}$).*

---

### 3. ¿Comparten número de i-nodo alguno de los ficheros o directorios creados? Incluye en tu respuesta el comportamiento ante un directorio, específicamente de las entradas ocultas del directorio `subdir` (puedes usar para ello `stat`, o la combinación de modificadores `-i` y `-a` de `ls`).

#### **Entre los ficheros regulares y enlaces creados:**
- **`fichero2` y `enlaceH` comparten el mismo número de nodo-i.**
  - Esto se debe a que `enlaceH` es un **enlace duro** (*hard link*). Un enlace duro no duplica los datos ni crea un nodo-i nuevo, sino que crea una entrada de directorio adicional que apunta exactamente al mismo nodo-i que `fichero2`. Como consecuencia directa, el contador de enlaces de dicho nodo-i aumenta a **2** (`Links: 2`).
- Por el contrario:
  - `fichero1` tiene un nodo-i propio e independiente.
  - `enlaceS` tiene su propio número de nodo-i independiente, ya que es un **enlace simbólico** (*symbolic link*). Un enlace simbólico es un fichero especial cuyo contenido almacena la ruta hacia el fichero objetivo (la cadena `"fichero2"`, de ahí que su tamaño sea de 8 bytes).

#### **Comportamiento en directorios (entradas ocultas de `subdir`):**
Al examinar las entradas ocultas dentro de `subdir` (por ejemplo con `ls -lai subdir` o `stat subdir/.` y `stat subdir/..`):
- **La entrada `.` (directorio actual):**
  - **Comparte el mismo número de nodo-i que el propio directorio `subdir`.**
  - Es el enlace duro interno que referencia al propio directorio.
- **La entrada `..` (directorio padre):**
  - **Comparte el mismo número de nodo-i que el directorio contenedor superior** (el directorio donde se ejecutó el script).
  - Es el enlace duro que referencia al directorio padre en la jerarquía del árbol de directorios.

*(En sistemas UNIX/Linux estándar no está permitido a los usuarios crear enlaces duros a directorios mediante el comando `ln` para evitar ciclos infinitos en el sistema de archivos; sin embargo, el propio sistema de archivos los implementa y mantiene de forma interna a través de las entradas `.` y `..`).*

---

### 4. Muestra el contenido de `enlaceH` y de `enlaceS` utilizando el comando `cat`. A continuación, borra el fichero `fichero2` y repite el procedimiento. ¿Puedes acceder en ambos casos al contenido del fichero?

#### **Comportamiento observado:**

1. **Antes de borrar `fichero2`:**
   ```bash
   $ cat enlaceH
   0123456789
   $ cat enlaceS
   0123456789
   ```
   En ambos casos se visualiza el contenido `"0123456789"` sin problemas.

2. **Tras borrar `fichero2` (`rm fichero2`):**
   ```bash
   $ cat enlaceH
   0123456789
   $ cat enlaceS
   cat: enlaceS: No such file or directory
   ```
   - A través de **`enlaceH`**: **SÍ** se puede acceder al contenido del fichero.
   - A través de **`enlaceS`**: **NO** se puede acceder al contenido, produciendo el error `No such file or directory`.

#### **Explicación razonada:**
- **Enlace duro (`enlaceH`):**
  El comando `rm fichero2` invoca la llamada al sistema `unlink()`, eliminando únicamente la entrada de directorio correspondiente al nombre `fichero2` y decrementando el contador de enlaces del nodo-i (`st_nlink`) de 2 a 1. Como el contador de enlaces no ha llegado a 0, el nodo-i y los bloques de datos en disco se mantienen intactos. Dado que `enlaceH` apunta directamente a dicho nodo-i, el fichero sigue existiendo y su contenido permanece plenamente accesible.
- **Enlace simbólico (`enlaceS`):**
  Un enlace simbólico no apunta al nodo-i, sino que contiene una referencia de texto con el nombre o ruta del fichero original (`"fichero2"`). Al intentar leerlo con `cat`, el sistema operativo intenta resolver esa ruta. Como la entrada `fichero2` ya no existe en el directorio, el enlace simbólico queda "roto" o "huérfano" (*dangling link*), provocando el fallo al abrirlo (`ENOENT`).

---

### 5. Utiliza la orden `touch` para modificar las fechas de acceso y modificación del fichero `enlaceH`. ¿Qué cambios se observan en la salida de `stat` tras su ejecución? Investiga a través de la página de manual para modificar únicamente una de dichas fechas (modificación o acceso).

#### **Efecto de ejecutar `touch enlaceH`:**
Al ejecutar `touch enlaceH`, se observan los siguientes cambios en la salida de `stat`:
1. **Fecha de último acceso (`Access` / *atime*):** Se actualiza a la fecha y hora actuales en que se ejecutó la orden.
2. **Fecha de última modificación (`Modify` / *mtime*):** Se actualiza a la fecha y hora actuales en que se ejecutó la orden.
3. **Fecha de cambio de estado/metadatos (`Change` / *ctime*):** Se actualiza automáticamente por el sistema al instante actual, ya que se han alterado los metadatos del nodo-i.
4. **Impacto en `fichero2`:**
   Dado que `enlaceH` y `fichero2` comparten el mismo nodo-i, al consultar `stat fichero2` se comprueba que sus marcas de tiempo **también han cambiado de forma idéntica**. Las fechas residen en la estructura del nodo-i, no en el nombre del fichero.

#### **Modificar únicamente una de dichas fechas (según `man touch`):**

- **Para modificar únicamente la fecha de acceso (*atime*):**
  Se emplea la opción **`-a`** (*change only the access time*):
  ```bash
  touch -a enlaceH
  ```
  O fijando una fecha específica con la opción `-d` (*date string*) o `-t` (*timestamp*):
  ```bash
  touch -a -d "2025-01-01 12:00:00" enlaceH
  ```
  *Efecto:* Se actualiza únicamente la marca `Access` (y `Change` por la actualización de metadatos del nodo-i), mientras que la fecha `Modify` permanece invariable.

- **Para modificar únicamente la fecha de modificación (*mtime*):**
  Se emplea la opción **`-m`** (*change only the modification time*):
  ```bash
  touch -m enlaceH
  ```
  O fijando una fecha específica:
  ```bash
  touch -m -d "2025-01-01 12:00:00" enlaceH
  ```
  *Efecto:* Se actualiza únicamente la marca `Modify` (y `Change`), mientras que la fecha `Access` permanece invariable.

*(Nota: La fecha de cambio de estado `Change` / *ctime* no puede ser forzada o modificada arbitrariamente por el usuario mediante flags de `touch`, ya que el sistema operativo la actualiza de forma automática cada vez que se produce cualquier modificación sobre el contenido o los metadatos del nodo-i).*
