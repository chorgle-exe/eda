#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int fibonacciRec(int n) {

	if (n == 1) {
		return 0;
	}
	if (n == 2) {
		return 1;
	}
    int k=10;
    while(k>1){
        k=k-1;
    }
	return fibonacciRec(n - 1) + fibonacciRec(n - 2);
}

int fibonacciInt(int n){
        int t1=0;
        int t2=1;
        int proximo=0;

        if (n == 1) {
            return t1;
        }
        if (n == 2) {
            return t2;
        }


    for(int i = 3; i<=n; i++){
        proximo = t1 + t2;
        t1=t2;
        t2=proximo;
        int k=10;
        while(k>1){
            k=k-1;
        }
    }
    return proximo;

}


int main(void) {
	struct timespec inicio, fim;
	clock_gettime(CLOCK_MONOTONIC, &inicio);

	fibonacciRec(42);

	clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo_decorrido = (fim.tv_sec - inicio.tv_sec)+ (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("tempo do rec %.6f em segundos\n",tempo_decorrido);

    // ITERATIVO

    clock_gettime(CLOCK_MONOTONIC, &inicio);

	fibonacciInt(42);

	clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo_decorrido2 = (fim.tv_sec - inicio.tv_sec)+ (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("tempo do it %.6f em segundos\n",tempo_decorrido2);
	return 0;
	
}