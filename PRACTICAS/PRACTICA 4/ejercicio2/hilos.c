#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_HILOS_DEFECTO 10 // Número de hilos a crear si no se especifica por línea de comandos

// Estructura para empaquetar los argumentos que recibirá cada hilo
struct thread_args {
    int num;        // Número identificativo asignado al hilo
    char prioridad; // 'P' si es par (prioritario) o 'N' si es impar (no prioritario)
};

// Función de inicio ejecutada concurrentemente por cada hilo
void *thread_usuario(void *arg)
{
    // Hacemos cast del puntero genérico void* al tipo estructurado correspondiente
    struct thread_args *args = (struct thread_args *)arg;

    // Copiamos los valores a variables locales en la pila (stack) del hilo
    int num = args->num;
    char prioridad = args->prioridad;

    // Liberamos la memoria dinámica reservada en heap para evitar fugas (memory leaks)
    free(args);

    // pthread_self() devuelve el identificador POSIX (pthread_t) del hilo en ejecución
    pthread_t id = pthread_self();

    // Mostramos por salida estándar el identificador de hilo, su número y su prioridad
    printf("Hilo [TID: %lu] - Número: %d, Prioridad: %c\n", (unsigned long)id, num, prioridad);

    return NULL;
}

int main(int argc, char *argv[])
{
    // Permitimos pasar opcionalmente el número de hilos como primer argumento del programa
    int num_hilos = (argc > 1) ? atoi(argv[1]) : NUM_HILOS_DEFECTO;
    if (num_hilos <= 0) {
        fprintf(stderr, "Uso: %s [num_hilos > 0]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Reservamos en memoria dinámica el array para almacenar los identificadores de los hilos
    pthread_t *hilos = malloc(num_hilos * sizeof(pthread_t));
    if (hilos == NULL) {
        perror("Error al reservar memoria para array de hilos");
        return EXIT_FAILURE;
    }

    // Bucle para crear e inicializar cada uno de los hilos
    for (int i = 0; i < num_hilos; i++) {
        // Reservamos memoria dinámica independiente en el heap para los argumentos de cada hilo
        struct thread_args *args = malloc(sizeof(struct thread_args));
        if (args == NULL) {
            perror("Error al reservar memoria para los argumentos del hilo");
            free(hilos);
            return EXIT_FAILURE;
        }

        // Inicializamos los argumentos: los pares son prioritarios ('P') y los impares no ('N')
        args->num = i;
        args->prioridad = (i % 2 == 0) ? 'P' : 'N';

        // pthread_create retorna 0 en éxito o código de error directamente (no utiliza errno)
        int res = pthread_create(&hilos[i], NULL, thread_usuario, args);
        if (res != 0) {
            fprintf(stderr, "Error al crear hilo %d: %s\n", i, strerror(res));
            free(args); // Liberamos la memoria del argumento si la creación del hilo falla
            free(hilos);
            return EXIT_FAILURE;
        }
    }

    // Bucle para esperar la finalización de todos los hilos creados
    for (int i = 0; i < num_hilos; i++) {
        // pthread_join bloquea al hilo principal hasta que el hilo indicado termina
        int res = pthread_join(hilos[i], NULL);
        if (res != 0) {
            fprintf(stderr, "Error al esperar hilo %d: %s\n", i, strerror(res));
        }
    }

    // Liberamos el array de identificadores reservado en el heap
    free(hilos);

    return EXIT_SUCCESS;
}
