#include "StepA.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
/// @brief 

void StepC(int intervalo,int duracion ){
    
  int petitions, total, available;
  float min;
  int iteracion = duracion/intervalo;  
  char linea[256];    // Número de veces que se ejecutará el bucle

  for (int i = 0; i < iteracion; i++) {
    FILE *disk = fopen("/proc/diskstats", "r");
    if (disk == NULL) {
      perror("Error al abrir el archivo /proc/diskstats");
    }
    // Leer y cerrar el archivo /proc/diskstats aquí si es necesario.
    
    char diskstats_line[256];
    fscanf(disk, "%s", diskstats_line);
    if (fgets(diskstats_line, sizeof(diskstats_line), disk) == NULL) {
      perror("Error al leer la línea de /proc/diskstats");

    }
    sscanf(diskstats_line,"%*d %*d %*s %d", &petitions);
    fclose(disk);
    FILE *mem = fopen("/proc/meminfo", "r");
    if (mem == NULL) {
      perror("Error al abrir el archivo /proc/meminfo");

    }

    while (fscanf(mem, "%s", linea) != EOF) {
      if (strcmp(linea, "MemTotal:") == 0) {
        fscanf(mem, "%d", &total);
      } else if (strcmp(linea, "MemAvailable:") == 0) {
        fscanf(mem, "%d", &available);
      }
    }

    fclose(mem);

    FILE *avg = fopen("/proc/loadavg", "r");
    if (avg == NULL) {
      perror("Error al abrir el archivo /proc/loadavg");
    }

    fscanf(avg, "%f", &min);
    fclose(avg);

    printf("Peticiones a disco: %d\n", petitions);
    printf("Memoria disponible / total: %d / %d\n", available, total);
    printf("Carga en el último minuto: %.2f\n", min);
    printf("[Pausa de %d segundos]\n\n", intervalo);
    //sleep(intervalo);
    sleep((unsigned int)intervalo);
  }
}


