#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t launch_command(char** argv){
    pid_t pid = fork();

    // El hijo sustituye su programa por el comando recibido.
    if (pid == 0) {
        execvp(argv[0], argv);
        perror("execvp");
        _exit(EXIT_FAILURE);
    }

    // El padre devuelve el PID sin esperar al hijo.
    if (pid == -1) {
        perror("fork");
    }

    return pid;
}



char **parse_command(const char *cmd) {
    // Allocate space for the argv array (initially with space for 10 args)
    size_t argv_size = 10;
    const char *end;
    size_t arg_len; 
    int arg_count = 0;
    const char *start = cmd;
    char **argv = malloc(argv_size * sizeof(char *));

    if (argv == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (*start && isspace(*start)) start++; // Skip leading spaces

    while (*start) {
        // Reallocate more space if needed
        if (arg_count >= argv_size - 1) {  // Reserve space for the NULL at the end
            argv_size *= 2;
            argv = realloc(argv, argv_size * sizeof(char *));
            if (argv == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        // Find the start of the next argument
        end = start;
        while (*end && !isspace(*end)) end++;

        // Allocate space and copy the argument
        arg_len = end - start;
        argv[arg_count] = malloc(arg_len + 1);

        if (argv[arg_count] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(argv[arg_count], start, arg_len);
        argv[arg_count][arg_len] = '\0';  // Null-terminate the argument
        arg_count++;

        // Move to the next argument, skipping spaces
        start = end;
        while (*start && isspace(*start)) start++;
    }

    argv[arg_count] = NULL; // Null-terminate the array

    return argv;
}


// Liberar cada cadena y el array reservado por parse_command.
void free_command(char **cmd_argv) {
    int i;

    for (i = 0; cmd_argv[i] != NULL; i++) {
        free(cmd_argv[i]);
    }
    free(cmd_argv);
}

// Ejecutar los comandos de un fichero en orden o todos en segundo plano.
int run_file(const char *filename, int background) {
    enum { MAX_COMMANDS = 100 };

    // FILE * permite leer el fichero de comandos línea a línea con fgets.
    FILE *file = fopen(filename, "r");

    // En modo -b, guardar aquí los PID para relacionarlos con cada comando.
    pid_t pids[MAX_COMMANDS];
    char line[1024];
    int command_count = 0;

    if (file == NULL) {
        perror("fopen");
        return -1;
    }

    // Leer y lanzar cada línea del fichero como un comando independiente.
    while (fgets(line, sizeof(line), file) != NULL) {
        char **cmd_argv;
        pid_t pid;
        size_t line_length;

        // fgets conserva el salto de línea, que se elimina manualmente.
        line_length = strlen(line);
        if (line_length > 0 && line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
        }

        // No crear procesos para líneas vacías.
        if (line[0] == '\0') {
            continue;
        }
        if (command_count == MAX_COMMANDS) {
            fprintf(stderr, "Too many commands (maximum %d)\n", MAX_COMMANDS);
            fclose(file);
            return -1;
        }

        printf("@@ Running command #%d: %s\n", command_count, line);

        // Convertir la línea en el formato argv que necesita execvp.
        cmd_argv = parse_command(line);

        // launch_command crea el proceso; después ya podemos liberar argv.
        pid = launch_command(cmd_argv);
        free_command(cmd_argv);

        // Si fork() falla, no podemos continuar con el resto de comandos.
        if (pid == -1) {
            fclose(file);
            return -1;
        }
        pids[command_count] = pid;

        // Sin -b, esperar antes de leer el siguiente comando.
        if (!background) {
            int status;

            // Como solo hay un hijo activo, wait espera directamente a ese hijo.
            if (wait(&status) == -1) {
                perror("wait");
                fclose(file);
                return -1;
            }
            printf("@@ Command #%d terminated (pid: %ld, status: %d)\n", command_count, (long)pid, status);
        }
        command_count++;
    }
    fclose(file);

    if (background) {
        int finished = 0;

        // Con -b, esperar a todos los hijos después de lanzarlos.
        while (finished < command_count) {
            int status;
            pid_t pid = wait(&status);
            int command_number;

            if (pid == -1) {
                perror("wait");
                return -1;
            }

            // Buscar qué número de comando corresponde al PID terminado.
            for (command_number = 0; command_number < command_count; command_number++) {
                if (pids[command_number] == pid) {
                    break;
                }
            }

            printf("@@ Command #%d terminated (pid: %ld, status: %d)\n",
                   command_number, (long)pid, status);
            finished++;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    char *command = NULL;
    char *filename = NULL;
    int background = 0;
    int option;

    // Procesar las opciones -x, -s y -b.
    while ((option = getopt(argc, argv, "x:s:b")) != -1) {
        switch (option) {
            case 'x':
                command = optarg;
                break;
            case 's':
                filename = optarg;
                break;
            case 'b':
                background = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s -x \"command\" | -s file [-b]\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    // -b solo tiene sentido cuando se usa junto con -s.
    if (optind != argc || (command == NULL && filename == NULL) ||
        (command != NULL && filename != NULL) ||
        (background && filename == NULL)) {
        fprintf(stderr, "Usage: %s -x \"command\" | -s file [-b]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (command != NULL) {
        char **cmd_argv;
        int status;
        pid_t pid;

        // Ejecutar el comando de -x y esperar a que termine.
        cmd_argv = parse_command(command);
        pid = launch_command(cmd_argv);
        free_command(cmd_argv);

        if (pid == -1 || wait(&status) == -1) {
            perror("wait");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    else if (filename != NULL) {
        if (run_file(filename, background) == -1) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}