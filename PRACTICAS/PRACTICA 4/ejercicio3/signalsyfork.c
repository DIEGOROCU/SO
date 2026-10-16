#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


static pid_t child_pid;

// El temporizador mata al hijo si sigue ejecutándose después de 5 segundos.
void alarm_handler(int signal_number)
{
	(void) signal_number;
	kill(child_pid, SIGKILL);
}

int main(int argc, char **argv)
{
	struct sigaction action;
	pid_t pid;
	pid_t finished_pid;
	int status;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s command [arg ...]\n", argv[0]);
		return EXIT_FAILURE;
	}

	// Preparar el manejador que recibirá SIGALRM.
	action.sa_handler = alarm_handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	if (sigaction(SIGALRM, &action, NULL) == -1) {
		perror("sigaction");
		return EXIT_FAILURE;
	}

	pid = fork();
	if (pid == -1) {
		perror("fork");
		return EXIT_FAILURE;
	}

	if (pid == 0) {
		// El hijo ejecuta el comando desde argv[1], conservando sus argumentos.
		execvp(argv[1], &argv[1]);
		perror("execvp");
		_exit(EXIT_FAILURE);
	}

	child_pid = pid;

	// El padre ignora Ctrl-C y programa la alarma de cinco segundos.
	signal(SIGINT, SIG_IGN);
	alarm(5);

	// Esperar al hijo; SIGALRM puede interrumpir wait antes de que muera.
	do {
		finished_pid = wait(&status);
	} while (finished_pid == -1 && errno == EINTR);

	alarm(0);
	if (finished_pid == -1) {
		perror("wait");
		return EXIT_FAILURE;
	}

	if (WIFEXITED(status)) {
		printf("Child terminated normally with status %d\n", WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		printf("Child terminated by signal %d\n", WTERMSIG(status));
	}

	return EXIT_SUCCESS;
}
