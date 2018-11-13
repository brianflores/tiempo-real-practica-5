//Para compilar: gcc -lpthread ejercicio1.c -o ejecicio1

#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

//Para calcular tiempo (funcion robada de sistemas distribuidos y paralelos)
double dwalltime(){
  double sec;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}
void* latencia(void *id){
    double latencia = 0;
    double tiempoEjecucionAnterior, tiempoEjecucionActual;
    int num = 0;
    int idThread = *(int*)id;
    while (num < 1000){
        tiempoEjecucionActual = dwalltime(); //me devuelve el tiempo actual
        if(num != 0) latencia+= tiempoEjecucionActual - tiempoEjecucionAnterior - 0.01; //La primera vez no hay latencia, las otras veces la latencia sera la resta del tiempo actual con el anterior y el tiempo de espera
        num++;
        tiempoEjecucionAnterior = tiempoEjecucionActual;
        usleep(10000); //10000 microsegundos equivalen a una centesima de segundo.
    }
    printf("Latencia promedio del thread %d : %f", idThread, latencia/1000);
}

int main(int argc,char*argv[]){
    pthread_t threads[2];
    for (int id=0;id<2; id++) pthread_create(&threads[id],NULL,&latencia,(void*)&id);
    for (int id = 0; id<2; id++) pthread_join(threads[id],NULL);
}