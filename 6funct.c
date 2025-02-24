#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

/*OPERACION:
 *    A   *  B  -  B  *  C
 *        F     -     D
 *              E
 *
 *  (5+3)*(4+8) - [(4+8)*(10-7)] = 60
 */

// Variables globales
int sum1= 0;
int sum2= 0;
int sum3= 0;
int mult1= 0;
int mult2= 0;
int sum5 = 0;

pthread_t thread0, thread1, thread2, thread3, thread4;

void *funcionA(void *id) {
    	printf("Thread %ld: Ejecutando funcionD\n", (long) id);
    	int wait_time = rand() % 5 + 1;
    	sleep(wait_time);
    	printf("%d segundos antes de la ejecucion de sum4\n", wait_time);
    	sum1 = 5 + 3;
    	pthread_exit(NULL);
}

void *funcionB(void *id) {
    	printf("Thread %ld: Ejecutando funcionB\n", (long) id);
    	sum2 = 4 + 8; // Asignación directa a la variable global
    	pthread_exit(NULL);
}

void *funcionC(void *id) {
    	printf("Thread %ld: Ejecutando funcionC\n", (long) id);
    	sum3 = 10 - 7; // Asignación directa a la variable global
    	pthread_exit(NULL);
}
void funcionE(){
	pthread_join(thread3, NULL); //Esperar a que termine F
        pthread_join(thread4, NULL); //Esperar a que termine D
        sum5 = mult1 - mult2;

}

void *funcionD(void *id){
	pthread_join(thread1, NULL); //Esperar a que termine B
	pthread_join(thread2, NULL); //Esperar a que termine C

	printf("Thread %ld: Ejecutando funcionC\n", (long) id);
        mult2 = sum2 * sum3;
	pthread_exit(NULL);
}

void *funcionF(void *id) {
	pthread_join(thread0, NULL); //Esperar a que termine A
	pthread_join(thread1, NULL); //Esperar a que termine B

	printf("Thread %ld: Ejecutando funcionC\n", (long) id);
    	mult1 = sum1 * sum2; // Asignación directa a la variable global
    	pthread_exit(NULL);
}
int main() {
 
	clock_t start, end;
	double cpu_time_used;
	start = clock();

        srand(time(NULL));

    	//Hilos
    	pthread_create(&thread1, NULL, funcionB, (void *) (long) 1);
    	pthread_create(&thread2, NULL, funcionC, (void *) (long) 2);
    	pthread_create(&thread0, NULL, funcionA, (void *) (long) 0);
	pthread_create(&thread3, NULL, funcionD, (void *) (long) 3);
	pthread_create(&thread4, NULL, funcionF, (void *) (long) 4);
    	
 
    	funcionE();

    	// Mostrar resultado final
    	printf("Resultado: %d\n", sum5);

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    	printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);
    
    	return 0;
}
