#include "StepA.h"


#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void StepCjson() {
    cJSON *root = cJSON_CreateObject();
    char hostname[256];
    time_t current_time;
    struct tm *time_info;
    char time_string[50];

    // Obtener el nombre de la máquina
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("Error al obtener el nombre de la máquina");
 
    }

    // Obtener la fecha y hora actual
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);

    // Agregar campos al objeto JSON
    cJSON_AddStringToObject(root, "Nombre de la máquina", hostname);
    cJSON_AddStringToObject(root, "Fecha y hora", time_string);

    // Leer información de /proc/cpuinfo
    FILE *fCpu = fopen("/proc/cpuinfo", "r");
    if (fCpu == NULL) {
        perror("Error al abrir /proc/cpuinfo");
  
    }

    char Cpu[80];
    int cant = 0;
    while (cant < 3 && fgets(Cpu, sizeof(Cpu), fCpu) != NULL) {
        if (strstr(Cpu, "model name")) {
            cJSON_AddStringToObject(root, "Modelo de CPU", strchr(Cpu, ':') + 2);
            cant++;
        } else if (strstr(Cpu, "cpu cores")) {
            cJSON_AddStringToObject(root, "Número de cores", strchr(Cpu, ':') + 2);
            cant++;
        } else if (strstr(Cpu, "cache size")) {
            cJSON_AddStringToObject(root, "Tamaño de caché", strchr(Cpu, ':') + 2);
            cant++;
        }
    }

    fclose(fCpu);

    // Leer la versión del kernel desde /proc/version
    FILE *vers = fopen("/proc/version", "r");
    if (vers == NULL) {
        perror("Error al abrir /proc/version");

    }

    char vv[256];
    if (fgets(vv, sizeof(vv), vers) != NULL) {
        char *token = strtok(vv, " ");
        int count = 0;

        while (token != NULL) {
            count++;
            if (count == 3) {
                cJSON_AddStringToObject(root, "Versión del kernel", token);
                break;
            }
            token = strtok(NULL, " ");
        }
    }

    fclose(vers);

    // Leer el tiempo transcurrido desde /proc/uptime
    FILE *ftime = fopen("/proc/uptime", "r");
    if (ftime == NULL) {
        perror("Error al abrir /proc/uptime");

    }

    double uptime_seconds;
    if (fscanf(ftime, "%lf", &uptime_seconds) != 1) {
        perror("Error al leer /proc/uptime");
        fclose(ftime);

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

    char uptime_string[256];
    snprintf(uptime_string, sizeof(uptime_string), "%02ld:%02ld:%02ld:%02ld", dias, horas, minutos, segundos);
    cJSON_AddStringToObject(root, "Tiempo transcurrido desde el inicio del sistema", uptime_string);

    // Leer sistemas de archivos desde /proc/filesystems
    FILE *ffile = fopen("/proc/filesystems", "r");
    if (ffile == NULL) {
        perror("Error al abrir /proc/filesystems");
 
    }

    cJSON *filesystems = cJSON_CreateArray();
    char fsys[256];
    while (fgets(fsys, sizeof(fsys), ffile) != NULL) {
        cJSON_AddItemToArray(filesystems, cJSON_CreateString(fsys));
    }
    cJSON_AddItemToObject(root, "Sistemas de archivos soportados por el kernel", filesystems);

    fclose(ffile);

    // Leer lista de particiones desde /proc/partitions
    FILE *fpart = fopen("/proc/partitions", "r");
    if (fpart == NULL) {
        perror("Error al abrir /proc/partitions");

    }

    cJSON *partitions = cJSON_CreateArray();
    char part[256];
    // Saltar las dos primeras líneas que contienen encabezados
    fgets(part, sizeof(part), fpart);
    fgets(part, sizeof(part), fpart);
    while (fgets(part, sizeof(part), fpart) != NULL) {
        int major, minor, blocks;
        char device[256];

        if (sscanf(part, " %d %d %d %s", &major, &minor, &blocks, device) == 4) {
            cJSON *partition = cJSON_CreateObject();
            cJSON_AddStringToObject(partition, "Dispositivo", device);
            cJSON_AddNumberToObject(partition, "Major", major);
            cJSON_AddNumberToObject(partition, "Minor", minor);
            cJSON_AddNumberToObject(partition, "Tamaño", blocks);
            cJSON_AddItemToArray(partitions, partition);
        }
    }

    cJSON_AddItemToObject(root, "Lista de particiones", partitions);
    fclose(fpart);

    // Convertir el objeto JSON a una cadena JSON
    char *json_str = cJSON_Print(root);
    if (json_str == NULL) {
        cJSON_Delete(root);
        fprintf(stderr, "Error al convertir JSON a cadena");

    }

    // Imprimir la cadena JSON
    printf("%s\n", json_str);

    // Liberar memoria
    cJSON_Delete(root);
    free(json_str);


}
