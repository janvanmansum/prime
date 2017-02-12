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
#include <pthread.h>

#define MAX_NUM_THREADS 1000

/*
 * GLOBALE DATA
 */

/*
 * Array waarin voor index i true komt te staan als i priem is en anders false.
 */
bool *is_prime;

/*
 * Mutex om toegang to is_prime te regelen.
 */
pthread_mutex_t is_prime_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Het volgende te getal om te onderzoeken.
 */
int next = 0;

/*
 * Mutex om toegang tot next te regelen.
 */
pthread_mutex_t next_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Het grootste getal om te onderzoeken.
 */
int max_number;


/*
 * FUNCTIEDEFINITIES
 */

/*
 * Druk een hulptekst over de juiste aanroep van het programma af.
 */
void print_usage(void);

/*
 * Zoek naar priemgetallen door steeds de volgende "next" op te halen, totdat deze
 * groter is dan max_number.
 */
void *find_primes(void *id);

/*
 * Haalt het volgende te onderzoeken getal op.
 */
int get_next_index(void);

/*
 * Leg vast in de is_prime array dat n geen priemgetal is.
 */
void record_not_prime(int n);

/*
 * Let vast dat p priem is (en zijn veelvouden niet).
 */
void record_prime(int p);

/*
 * Alloceer ruimte voor de is_prime-array op de heap en inilialiseer de cellen op true.
 */
void init_is_prime(int size);

/*
 * Verwijder de array is_prime van de heap.
 */
void free_is_prime(void);


/*
 * Hoofdroutine.
 */
int main(int argc, char* argv[]) {
	pthread_t threads[MAX_NUM_THREADS];
	if (argc < 2) {
		fprintf(stderr, "Error: incorrect number of arguments.\n");
		print_usage();
	}
	max_number = atoi(argv[1]);
	int num_threads = argc == 3 ? atoi(argv[2]): 2;
	int rc;
	init_is_prime(max_number + 1);

	for (int i = 0; i < num_threads; ++i) {
		rc = pthread_create(&threads[i], NULL, find_primes, (void *)i);
	}

	for (int i = 0; i < num_threads; ++i) {
		rc = pthread_join(threads[i], NULL);
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
	free_is_prime();
	pthread_exit(NULL);
}

/*
 * Implementatie van hulpfuncties.
 */

void init_is_prime(int size) {
	is_prime = (bool *) malloc(sizeof(bool) * (size));
	is_prime[0] = is_prime[1] = false;
	for (int i = 2; i < size; ++i) is_prime[i] = true;
}

void free_is_prime(void) {
	free(is_prime);
}

pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

void output(char *s, int id) {
	pthread_mutex_lock(&output_mutex);
	printf(s, id);
	pthread_mutex_unlock(&output_mutex);
}

void *find_primes(void *id) {
	int n = get_next_index();
	while (n < max_number) {
		if (is_prime[n]) {
			int i;
			for (i = n - 1; i > 1; --i) {
				if (n % i == 0) {
					record_not_prime(n);
					break;
				}
			}
			if (i == 1)
				record_prime(n);
		}
		n = get_next_index();
	}
	pthread_exit(NULL);
}

int get_next_index() {
	pthread_mutex_lock(&next_mutex);
	int n = next;
	next++;
	pthread_mutex_unlock(&next_mutex);
	return n;
}

void record_not_prime(int n) {
	pthread_mutex_lock(&is_prime_mutex);
	is_prime[n] = false;
	pthread_mutex_unlock(&is_prime_mutex);
}

void record_prime(int p) {
	pthread_mutex_lock(&is_prime_mutex);
	for (int i = p * 2; i < max_number + 1; i += p) {
		is_prime[i] = false;
	}
	pthread_mutex_unlock(&is_prime_mutex);
}

void print_usage() {
	fprintf(stderr, "Usage: priem [max-number-to-test] [number-of-threads; default is 2]");
}

