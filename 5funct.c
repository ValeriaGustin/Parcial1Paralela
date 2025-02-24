#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

/*OPERACION:
 *    B  *  C   +   A
 *       D      +   A
 *              E
 *  (5+3)*(4+8) + (7-2) = 101
 *
 */ 

// Variables globales
int sum1= 0;
int sum2= 0;
int mult3= 0;
int sum4= 0;
int sum5= 0;
pthread_t thread0, thread1, thread2;

void *funcionA(void *id) {
	printf("Thread %ld: Ejecutando funcionD\n", (long) id);
    	int wait_time = rand() % 5 + 1;
    	sleep(wait_time);
    	printf("%d segundos antes de la ejecucion de sum4\n", wait_time);
    	sum4 = 7 - 2;
    	pthread_exit(NULL);
}

void *funcionB(void *id) {
    	printf("Thread %ld: Ejecutando funcionB\n", (long) id);
    	sum1 = 5 + 3;
    	pthread_exit(NULL);
}

void *funcionC(void *id) {
    	printf("Thread %ld: Ejecutando funcionC\n", (long) id);
    	sum2 = 4 + 8;
    	pthread_exit(NULL);
}
void funcionE(){
	pthread_join(thread2, NULL); //Esperar a que termine A
    	sum5 = mult3 + sum4;

}

void funcionD(){
	mult3 = sum1 * sum2;
    	funcionE();
}




int main() {
	srand(time(NULL));

    	// Crear hilos para calcular sum1 y sum2
    	pthread_create(&thread0, NULL, funcionB, (void *) (long) 0);
    	pthread_create(&thread1, NULL, funcionC, (void *) (long) 1);
    	pthread_create(&thread2, NULL, funcionA, (void *) (long) 2);

    	// Esperar a que terminen B y C
    	pthread_join(thread0, NULL);
    	pthread_join(thread1, NULL);
    
    	funcionD();

    	// Mostrar resultado final
    	printf("Resultado: %d\n", sum5);

    	return 0;
}

