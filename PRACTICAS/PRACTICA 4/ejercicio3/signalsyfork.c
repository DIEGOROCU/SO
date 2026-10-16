#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static pid_t child_pid;

// SIGALRM mata al hijo cuando supera los cinco segundos de ejecución.
void alarm_handler(int signal_number)
{
    // signal_number contiene el número de la señal recibida.
    (void) signal_number;
    // kill recibe el PID del proceso y la señal que debe enviarle.
    kill(child_pid, SIGKILL);
}

int main(int argc, char **argv)
{
    pid_t pid;
    int status;

    // argc es el número de argumentos y argv contiene sus textos.
    if (argc < 2) {
        // Sin un comando en argv[1], no hay ningún programa que ejecutar.
        fprintf(stderr, "Usage: %s command [arg ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // signal recibe la señal que se vigila y la función que se ejecutará.
    // SIGALRM es la señal que envía alarm cuando se cumple el tiempo.
    if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
        // SIG_ERR indica que no se pudo instalar el manejador.
        perror("signal");
        return EXIT_FAILURE;
    }

    // fork crea el hijo y devuelve un resultado distinto en cada proceso.
    pid = fork();
    if (pid == -1) {
        // -1 significa que fork no pudo crear el proceso hijo.
        perror("fork");
        return EXIT_FAILURE;
    }

    // El hijo entrara en este bloque y ejecutará el comando recibido en argv[1] con execvp.
    if (pid == 0) {
        // 0 identifica al hijo; &argv[1] es su nuevo array de argumentos.
        execvp(argv[1], &argv[1]);
        // execvp solo continúa aquí si no pudo ejecutar el comando (sustituyendo el programa del hijo).
        perror("execvp");
        _exit(EXIT_FAILURE);
    }

    // En el padre, pid contiene el PID del hijo recién creado.
    child_pid = pid;
    // alarm recibe segundos y enviará SIGALRM al finalizar ese tiempo.
    alarm(5);

    // wait guarda en status la causa de terminación del hijo.
    if (wait(&status) == -1) {
        // -1 indica que wait no pudo esperar al hijo.
        perror("wait");
        return EXIT_FAILURE;
    }

    // El hijo ya terminó, por lo que no necesitamos mantener la alarma.
    alarm(0);

    // WIFEXITED comprueba si el hijo terminó llamando a exit o return.
    if (WIFEXITED(status)) {
        printf("Child terminated normally with status %d\n", WEXITSTATUS(status));
    // WIFSIGNALED comprueba si el hijo terminó por una señal.
    } else if (WIFSIGNALED(status)) {
        printf("Child terminated by signal %d\n", WTERMSIG(status));
    }

    return EXIT_SUCCESS;
}
