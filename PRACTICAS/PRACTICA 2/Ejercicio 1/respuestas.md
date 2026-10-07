# Respuestas - Ejercicio 1

## Analisis y modificaciones del programa

### 1. Reemplazo de `getc()` por `fread()`

- `getc()` lee un unico caracter y devuelve su valor como un `int` (para poder representar el valor especial `EOF = -1` que indica fin de fichero o error).
- `fread()` permite leer bloques de bytes especificados por su tamaño y numero de elementos:
  ```c
  size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
  ```
  En la modificacion se utiliza:
  ```c
  while (fread(&c, sizeof(c), 1, file) == 1)
  ```
  donde `c` es una variable de tipo `unsigned char`. El bucle se mantiene activo mientras `fread` logre leer exactamente `1` elemento de 1 byte. Al alcanzar el fin de fichero (`EOF`) o producirse un error, devuelve `0`, concluyendo el bucle.

### 2. Reemplazo de `putc()` por `fwrite()`

- `putc()` escribe un unico caracter en el flujo y devuelve el caracter escrito o `EOF` en caso de error.
- `fwrite()` escribe bloques binarios de datos:
  ```c
  size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
  ```
  En la modificacion se utiliza:
  ```c
  ret = fwrite(&c, sizeof(c), 1, stdout);
  if (ret != 1) {
      fclose(file);
      err(3, "fwrite() failed!!");
  }
  ```
  Comprobando que devuelva `1`, que confirma la escritura correcta del elemento.

### 3. Comparativa entre ambas aproximaciones

- **`getc` / `putc`**: Estan orientadas al tratamiento de flujos como caracteres individuales y realizan una llamada conceptual por caracter (aunque internamente la biblioteca estandar implemente buffers en el objeto `FILE`).
- **`fread` / `fwrite`**: Estan disenadas para entrada/salida binaria y por bloques. Aunque en este primer ejercicio se lean de uno en uno (`nmemb = 1`), la interfaz de `fread`/`fwrite` permite facilmente leer y escribir buffers de mayor tamaño (`BUFFER_SIZE`), optimizando drasticamente el numero de operaciones y llamadas al sistema en comparacion con operaciones caracter a caracter.
