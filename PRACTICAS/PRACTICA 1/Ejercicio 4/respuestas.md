# Respuestas - Ejercicio 4

**1. Propósito de `clone_string()` y por qué no usar `strcpy()` o asignación directa:**
El propósito de `clone_string()` es reservar dinámicamente memoria (con `malloc`) con el tamaño exacto de la cadena a copiar y luego copiar sus caracteres.
*   **¿Por qué no usar `strcpy()` directamente?** Porque los campos de la estructura `passwd_entry_t` son punteros (`char*`). Para copiar usando `strcpy()`, el puntero de destino debe apuntar a un bloque de memoria previamente reservado y con tamaño suficiente. Si hacemos `strcpy(campo, token)` sin inicializar `campo`, se producirá un fallo de segmentación (segmentation fault) por intentar escribir en memoria no reservada o apuntada por un puntero "basura".
*   **¿Por qué no usar asignación `campo = cadena_existente`?** Si hacemos esto, solo copiamos el *puntero*, no la cadena en sí. Dado que la cadena original (`line`) va siendo procesada o puede sobreescribirse al leer la siguiente línea del archivo, todos los punteros acabarían apuntando a datos inválidos o modificados posteriormente. `clone_string()` crea una copia en memoria estricta y segura para ese dato.

*(Nota: en el código resuelto se ha reemplazado `clone_string` por la función estándar `strdup` que hace exactamente lo mismo).*

**2. ¿Qué tipo de modificaciones sufre la cadena tras invocaciones sucesivas de `strsep()`?**
La función `strsep()` avanza el puntero a la cadena y reemplaza el delimitador encontrado por el carácter nulo de terminación `'\0'`. Es decir, "trocea" la cadena original en memoria en subcadenas terminadas en nulo, alterando físicamente la cadena original en el proceso.
