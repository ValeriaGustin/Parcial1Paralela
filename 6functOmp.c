#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

/*OPERACION:
 *    A   *  B  -  B  *  C
 *        F     -     D
 *              E
 *
 *  (5+3)*(4+8) - [(4+8)*(10-7)] = 60
 */

// Variables globales
int sum1 = 0;
int sum2 = 0;
int sum3 = 0;
int mult1 = 0;
int mult2 = 0;
int sum5 = 0;

void funcionA() {
	int id = omp_get_thread_num();
    	printf("Thread %d: Ejecutando funcionA\n", id);
    	int wait_time = rand() % 5 + 1;
    	sleep(wait_time);
    	printf("%d segundos antes de la ejecucion de sum1\n", wait_time);
    	sum1 = 5 + 3;
}

void funcionB() {
	int id = omp_get_thread_num();
    	printf("Thread %d: Ejecutando funcionB\n", id);
    	sum2 = 4 + 8;
}

void funcionC() {
	int id = omp_get_thread_num();
    	printf("Thread %d: Ejecutando funcionC\n", id);
    	sum3 = 10 - 7;
}

void funcionD() {
    	int id = omp_get_thread_num();
    	printf("Thread %d: Ejecutando funcionD\n", id);
    	mult2 = sum2 * sum3;
}

void funcionF() {
    	int id = omp_get_thread_num();
    	printf("Thread %d: Ejecutando funcionF\n", id);
    	mult1 = sum1 * sum2;
}

void funcionE() {
    	sum5 = mult1 - mult2;
}

int main() {
    	clock_t start, end;
    	double cpu_time_used;
    	start = clock();

    	srand(time(NULL));

    	// Ejecutar funcionA, funcionB y funcionC en paralelo
    	#pragma omp parallel sections
    	{
        	#pragma omp section
        	{funcionA();}

        	#pragma omp section
        	{funcionB();}

        	#pragma omp section
        	{funcionC();}
    	}

    	// Ejecutar funcionF y funcionD en paralelo, despues de funcionA, funcionB y funcionC
    	#pragma omp parallel sections
    	{
        	#pragma omp section
		{funcionF();}

        	#pragma omp section
		{funcionD();}
    	}

    	// Ejecutar funcionE después de que funcionF y funcionD hayan terminado
    	funcionE();

    	// Mostrar resultado final
    	printf("Resultado: %d\n", sum5);

    	end = clock();
    	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    	printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);

    	return 0;
}
