#include "StepA.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/// @brief 

void StepB(){
   FILE *Stat;
  char stati[256];

  Stat = fopen("/proc/stat", "r");
  if (Stat == NULL) {
        printf("ERROR al abrir el archivo!\n");

  }

  while (fgets(stati, sizeof(stati), Stat)) {
    if (strncmp(stati, "cpu", 3) == 0) {
      char cpu_name[5];
      int user, nice, system, idle;
      sscanf(stati, "%s %d %d %d %d", cpu_name, &user, &nice, &system, &idle);

      if (strcmp(cpu_name, "cpu") == 0) {
        printf("Tiempo de CPU para usuario: %d\n", user);
        printf("Tiempo de CPU para sistema: %d\n", system);
        printf("Tiempo de CPU en estado idle: %d\n", idle);
      }
    } else if (strncmp(stati, "ctxt", 4) == 0) {
      int contexto;
      sscanf(stati, "ctxt %d", &contexto);
      printf("Número de cambios de contexto: %d\n", contexto);
    } else if (strncmp(stati, "processes", 9) == 0) {
      int procesos;
      sscanf(stati, "processes %d", &procesos);
      printf("Número de procesos creados: %d\n", procesos);
      }
  }

  fclose(Stat);

}