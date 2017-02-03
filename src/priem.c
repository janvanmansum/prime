/*
 ============================================================================
 Name        : priem.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/*
 * Globale data
 */

/*
 * Array waarin voor index i true komt te staan als i priem is en anders false.
 */
bool *is_prime;

/*
 * Het grootste getal
 */
int max_number;

/*
 * Druk een hulptekst over de juiste aanroep van het programma af.
 */
void print_usage(void);

void determine_and_record_primeness(int n);

void init_primeness(int size);

void free_primeness(void);

int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Error: incorrect number of arguments.\n");
		print_usage();
	}
	max_number = atoi(argv[1]);
	init_primeness(max_number + 1);
	for (int i = 0; i < max_number + 1; ++i) {
		determine_and_record_primeness(i);
	}
	printf("Primes: ");
	bool first = true;
	for (int i = 0; i < max_number + 1; ++i) {
		if (is_prime[i]) {
			if (!first) printf(", ");
			printf("%d", i);
			first = false;
		}
	}
	free_primeness();
	return EXIT_SUCCESS;
}

void init_primeness(int size) {
	is_prime = (bool *) malloc(sizeof(bool) * (size));
	for (int i = 0; i < size; ++i) is_prime[i] = true;
}

void free_primeness(void) {
	free(is_prime);
}

void determine_and_record_primeness(int n) {
	if (!is_prime[n]) return;

	int i;
	//int from = ceil(sqrt(n));
	for (i = n - 1; i > 1; --i) {
		if (n % i == 0) {
			is_prime[n] = false;
			return;
		}
	}
	is_prime[n] = i == 1;
	if (is_prime[n]) {
		for (i = 2 * n; i < max_number + 1; i += n) {
//			printf("Setting to NOT prime: %d\n", i);
			is_prime[i] = false;
		}
	}
}

int get_next_index() {
	// Get index-mutex
	// Copy index
	// Increment index
	// Unlock mutext
}

void record_prime(int p) {
	// Get is_prime mutex
	// Update array
	// Unlock
}

void print_usage(void) {
	fprintf(stderr, "Usage: priem [max-number-to-test] [number-of-threads; default is 2]");


}

