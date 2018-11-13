#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main(int argc,char*argv[]){
    FILE *fp;
    char* filename = "datosSensor.txt";
    fp = fopen(filename, "r");
    mkfifo("pipe", 0666);
    int fd;
    if (fp != NULL) {
        fd = open("pipe", O_WRONLY); 
        char *buffer;
        size_t bufsize = 50;
        size_t characters;
        int tiempo;
        char temperatura[10];
        buffer = (char *)malloc(bufsize * sizeof(char));
        while ((characters = getline(&buffer, &bufsize, fp)) != -1) {
            sscanf(buffer, "%d %s", &tiempo, temperatura);
            printf("%d  %s\n", tiempo, temperatura);
            usleep(tiempo/1000); // Pasa de nanosegundo a microsegundos y se duerm
            //sprintf(temp, "%d", temperatura); // Convierte la temperatura a string para poder enviar
            write(fd, temperatura, strlen(temperatura));
        }
    free(buffer);
    fclose(fp);
    }
}