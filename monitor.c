#include <pthread.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

pthread_t thread1,thread2;
pthread_mutex_t semaforo;
int temperaturaGlobal[3];
int cantidadMediciones = 0;

void* tarea1(){
    int fd, temp;
    char temperatura[10];
    fd = open("pipe", O_RDONLY); 
    while (1){
        read(fd, temperatura, strlen(temperatura)); //Lee la temperatura del pipe
        sscanf(temperatura, "%d", &temp); // Convierte la temperatura a entero
        if (temp > 90) printf("La temperatura es mayor a 90 grados!\n");
        else {
            pthread_mutex_lock(&semaforo); // Espera a poder escribir el valor de temperatura
            temperaturaGlobal[cantidadMediciones % 3] = temp;
            pthread_mutex_unlock(&semaforo); 
            cantidadMediciones++;
        }
    }
}

void* tarea2(){
    int suma;
    double promedio;
    while (cantidadMediciones<3){}; // Espera a que haya al menos 3 mediciones validas para calcular el promedio
    while(1){
        suma = 0;
        pthread_mutex_lock(&semaforo);
        for (int i=0; i<3; i++) suma+= temperaturaGlobal[i];
        promedio = suma/3;
        printf("El promedio de las ultimas 3 mediciones es %f",promedio);
    }    
}

int main(void){
    pthread_mutex_init(&semaforo, NULL);
    pthread_create(&thread1,NULL,&tarea1,0);
    pthread_create(&thread2,NULL,&tarea2,0);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_mutex_destroy(&semaforo);
}