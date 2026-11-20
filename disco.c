#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

struct argumento {
	int id;
	bool isVip;
};

// Mutex global
pthread_mutex_t mutex;
pthread_cond_t cond_vip;
pthread_cond_t cond_normal;
int clientesDentro;
int vipsEsperando;

void enter_vip_client(int id)
{
	pthread_mutex_lock(&mutex);

	vipsEsperando++;

	while (clientesDentro >= CAPACITY) {
		pthread_cond_wait(&cond_vip, &mutex);
	}

	clientesDentro++;
	vipsEsperando--;
	printf("Client %2d (  vip  ) enters disco. Inside: %d\n", id, clientesDentro);

	pthread_mutex_unlock(&mutex);
}

void enter_normal_client(int id)
{
	pthread_mutex_lock(&mutex);

	while (clientesDentro >= CAPACITY || vipsEsperando > 0) {
		pthread_cond_wait(&cond_normal, &mutex);
	}

	clientesDentro++;
	printf("Client %2d (not vip) enters disco. Inside: %d\n", id, clientesDentro);

	pthread_mutex_unlock(&mutex);

}

void dance(int id, int isVip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isVip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isVip)
{

	pthread_mutex_lock(&mutex);

	clientesDentro--;
	printf("Client %2d (%s) leaves disco. Inside: %d\n", id, VIPSTR(isVip), clientesDentro);

	if (vipsEsperando > 0) {
		pthread_cond_signal(&cond_vip);
	}
	else {
		pthread_cond_signal(&cond_normal);
	}

	pthread_mutex_unlock(&mutex);
}

void *client(void *arg)
{
	struct argumento *entrada = (struct argumento *)arg;
	int id = entrada->id;
	bool isVip = entrada->isVip;
	free(entrada);

	if (isVip) {
		enter_vip_client(id);
	}
	else {
		enter_normal_client(id);
	}
	dance(id, isVip);
	disco_exit(id, isVip);

	return NULL;
}

int main(int argc, char *argv[])
{

	if (argc < 2) {
        fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
        exit(1);
    }

    FILE *archivo = fopen(argv[1], "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return 1;
    }

	// Leemos la cantidad de clientes
	int cantidad;
	if (fscanf(archivo, "%d", &cantidad) != 1 || cantidad < 0) {
		fclose(archivo);
        perror("Error al leer los clientes");
    }

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_vip, NULL);
    pthread_cond_init(&cond_normal, NULL);

	// Inicializamos la lista de clientes
	clientesDentro = 0;
	vipsEsperando = 0;
	int i = 0;
	int *listaClientes = malloc(cantidad * sizeof(int));
	pthread_t *thread_ids = malloc(cantidad * sizeof(pthread_t));
	// Leemos los clientes
	for (i = 0; i < cantidad ; i++){
		if (fscanf(archivo, "%d", &listaClientes[i]) != 1 || listaClientes[i] < 0 || listaClientes[i] > 1) {
			free(listaClientes);
			fclose(archivo);
        	perror("Error al leer el cliente ");
    	}
		else {
			struct argumento* arg = malloc(sizeof(struct argumento));
			arg->id = i;
			arg->isVip = listaClientes[i];
			pthread_create(&thread_ids[i], NULL, client, arg);
		}
	}

	for (i = 0; i < cantidad; i++) {
		pthread_join(thread_ids[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond_vip);
    pthread_cond_destroy(&cond_normal);
	free(listaClientes);
	free(thread_ids);
    fclose(archivo);

    return 0;
}