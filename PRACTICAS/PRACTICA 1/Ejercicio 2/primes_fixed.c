/**
* This program calculates the sum of the first n prime
* numbers. Optionally, it allows the user to provide as argument the
* value of n, which is 10 by default.
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
* This function takes an array of integers and returns the sum of its n elements.
*/
int sum(int *arr, int n);

/**
* This function fills an array with the first n prime numbers.
*/
void compute_primes(int* result, int n);

/**
* This function returns 1 if the integer provided is a prime, 0 otherwise.
*/
int is_prime(int x);

int main(int argc, char **argv) {
    int n = 10; // by default the first 10 primes
    // FIX 1: Use == for comparison, not = (assignment)
    if(argc == 2) {
        // FIX 2: Store the result of atoi, and read from argv[1] (argv[0] is program name)
        n = atoi(argv[1]);
    }

    int* primes = (int*)malloc(n*sizeof(int));
    compute_primes(primes, n);

    int s = sum(primes, n);
    printf("The sum of the first %d primes is %d\n", n, s);

    free(primes);
    return 0;
}

int sum(int *arr, int n) {
    int i;
    // FIX 3: Initialize total to 0
    int total = 0;
    for(i=0; i<n; i++) {
        // FIX 4: Use += instead of =+ (which is just assigning positive arr[i])
        total += arr[i];
    }
    return total;
}

void compute_primes(int* result, int n) {
    int i = 0;
    int x = 2;
    while(i < n) {
        if(is_prime(x)) {
            result[i] = x;
            i++;
        }
        // FIX 5: Increment x unconditionally, otherwise infinite loop when x is not prime
        x++; 
    }
    return;
}

int is_prime(int x) {
    // FIX 6: Handle 2 explicitly, as 2 % 2 == 0 would return 0 otherwise
    if(x == 2) return 1;
    
    if(x % 2 == 0) {
        return 0;
    }
    for(int i=3; i<x; i+=2) {
        if(x % i == 0) {
            return 0;
        }
    }
    // FIX 7: Return 1 if no divisors were found
    return 1;
}
