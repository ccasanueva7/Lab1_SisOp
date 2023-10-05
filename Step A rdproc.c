#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main() {
    FILE *vers;
    char vv[256];
    FILE *fCpu;
    char Cpu[80];
    int cant=0;
    FILE *ftime;
    FILE *ffile;
    char fsys[256];
    FILE *fpart;
    char part[256];
    char hostname[256];
    time_t current_time;
    struct tm *time_info;
    char time_string[50];
    
    
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("Error al obtener el nombre de la máquina");
        return 1;
    }

    //fecha y hora actual
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);

    // Imprimir la cabecera
    printf("Nombre de la máquina: %s\n", hostname);
    printf("Fecha y hora : %s\n", time_string);
    
    //abrimos archivo cpuinfo
    fCpu= fopen("/proc/cpuinfo","r");
    if((fCpu= fopen("/proc/cpuinfo","r"))==NULL){
        printf("ERROR!");
        return 1;
    }
    cant =0;
    //sacamos la info de cpuinfo
    while(cant<3){
        fgets ( Cpu, 80, fCpu );
        if(!strncmp(Cpu,"model name",strlen("model name"))){
            printf("%s", Cpu); cant++;
        }
        else if(!strncmp(Cpu,"cpu cores",strlen("cpu cores"))){
            printf("%s", Cpu); cant++;
        }
        else if(!strncmp(Cpu,"cache size",strlen("cache size"))){
            printf("%s", Cpu); cant++;
        }
    }
    fclose(fCpu);
    //version del kernel 
    vers = fopen("/proc/version","r");
    if ((vers=fopen ("/proc/version","r"))==NULL){
        printf("Error!");
        return 1;
    } 
    
   
    if (fgets(vv, sizeof(vv), vers) != NULL) {
        // Divide la línea en palabras para obtener la versión del kernel
        char *token = strtok(vv, " ");
        int count = 0;
        
        while (token != NULL) {
            count++;
            if (count == 3) {
                printf("Versión del kernel: %s\n", token);
                break;
            }
            token = strtok(NULL, " ");
        }
    }

    fclose(vers);
    //tiempo transcurrido
    ftime= fopen("/proc/uptime","r");
    if((ftime= fopen("/proc/uptime","r"))==NULL){
        printf("ERROR!");
        return 1;
    }
    

    double uptime_seconds;
    if (fscanf(ftime, "%lf", &uptime_seconds) != 1) {
        perror("Error al leer el archivo");
        fclose(ftime);
        return 1;
    }

    fclose(ftime);

    unsigned long uptime = (unsigned long)uptime_seconds;
    unsigned long dias, horas, minutos, segundos;

    dias = uptime / 86400;
    uptime %= 86400;
    horas = uptime / 3600;
    uptime %= 3600;
    minutos = uptime / 60;
    segundos = uptime % 60;

    printf("Tiempo transcurrido desde el inicio del sistema: %02ld:%02ld:%02ld:%02ld\n", dias, horas, minutos, segundos);
    
    //lista de filesystems
    ffile= fopen("/proc/filesystems","r");
    if((ftime= fopen("/proc/filesystems","r"))==NULL){
        printf("ERROR!");
        return 1;
    }

    printf("Sistemas de archivos soportados por el kernel:\n");

    while (fgets(fsys, sizeof(fsys), ffile) != NULL) {
        printf("%s", fsys);
    }

    fclose(ffile);
    
    
    //lista de partciones 
    
    fpart= fopen("/proc/partitions","r");
    if((fpart= fopen("/proc/partitions","r"))==NULL){
        printf("ERROR!");
        return 1;
    }
    

    printf("Lista de particiones:\n");

    // Saltar las dos primeras líneas que contienen encabezados
    fgets(part, sizeof(part), fpart);
    fgets(part, sizeof(part), fpart);

    while (fgets(part, sizeof(part), fpart) != NULL) {
        int major, minor, blocks;
        char device[256];

        if (sscanf(part, " %d %d %d %s", &major, &minor, &blocks, device) == 4) {
            printf("Dispositivo: %s (Major: %d, Minor: %d, Tamaño: %d )\n", device, major, minor, blocks);
        }
    }

    fclose(fpart);

    
    return 0;

}