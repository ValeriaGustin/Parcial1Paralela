#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

/*OPERACION:
 *  (5+3)*(4+8) + (7-2) = 101
 *
 */

// Variables globales
int sum1 = 0;
int sum2 = 0;
int mult3 = 0;
int sum4 = 0;
int sum5 = 0;

void funcionA() {
	int id = omp_get_thread_num();
    	printf("Thread %d: Ejecutando funcionA\n", id);
    	int wait_time = rand() % 5 + 1;
    	sleep(wait_time);
    	printf("%d segundos antes de la ejecucion de sum4\n", wait_time);
    	sum4 = 7 - 2;
}

void funcionB() {
	int id = omp_get_thread_num();
    	printf("Thread %d: Ejecutando funcionB\n", id);
    	sum1 = 5 + 3;
}

void funcionC() {
    	int id = omp_get_thread_num();
    	printf("Thread %d: Ejecutando funcionC\n", id);
    	sum2 = 4 + 8;
}

void funcionE() {
    	sum5 = mult3 + sum4;
}

void funcionD() {
    	mult3 = sum1 * sum2;
    	funcionE();
}

int main() {
	srand(time(NULL));

	// Crear hilos para calcular sum1, sum2 y sum4 en paralelo
    	#pragma omp parallel sections
    	{
		#pragma omp section
        	{funcionB();}

       		#pragma omp section
        	{funcionC();}

        	#pragma omp section
        	{funcionA();}
    	}

    	// Una vez que todas las secciones paralelas han terminado, ejecutar funcionD
    	funcionD();

    	// Mostrar resultado final
    	printf("Resultado: %d\n", sum5);

    	return 0;
}
