#include <stdio.h>
#include <stdlib.h>

// is_prime comprueba si x es primo.
int is_prime(int x); 
void compute_primes(int result[], int n);
int sum(int arr[], int n);

int main() {
    int primes[10]; // Array de tamaño fijo reservado en la pila
    // Se pasa 'primes' por referencia (decaimiento a puntero).
    compute_primes(primes, 10);
    int total = sum(primes, 10);
    printf("The sum of the first 10 primes is: %d\n", total);
    return 0; // Código 0 indica éxito en la ejecución
}

int sum(int arr[], int n) {
    int i;
    int total = 0; // Obligatorio inicializar a 0 para no arrastrar basura de memoria
    for(i=0; i<n; i++) {
        total += arr[i]; // Equivalente a total = total + arr[i]
    }
    return total;
}

void compute_primes(int result[], int n) {
    int i = 0;
    int x = 2;
    while(i < n) {
        if(is_prime(x)) {
            result[i] = x;
            i++;
        }
        x++; // Siempre se incrementa, de lo contrario causaría bucle infinito
    }
    return;
}

int is_prime(int x) {
    if(x == 2) return 1; // Excepción explícita para el único primo par
    
    // El operador % (módulo) obtiene el resto de la división
    if(x % 2 == 0) {
        return 0;
    }
    // i+=2 va comprobando solo impares para ahorrar iteraciones
    for(int i=3; i<x; i+=2) {
        if(x % i == 0) {
            return 0;
        }
    }
    return 1;
}
