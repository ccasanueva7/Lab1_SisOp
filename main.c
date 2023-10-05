#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "StepA.h"
#include <getopt.h>  



const char* programa;
int intervalo = 0, duracion = 0, pid = 0; //Intervalos 

void print_op(FILE* stream, int exit_code)
{
  fprintf (stream, "Uso: %s [opcion] [argumento]\n", programa);
  fprintf (stream,
    "Opciones:\n"
    " -s      --stats      - tiempos del CPU.\n"
    " -l a b  --interval a b   -estadísitcas del disco y la memoria repetidamente\n"                    
    "                        por el intervalo a  b.\n"
    " -p pid  --pid pid    - Muestra info sobre los file descriptors del\n"
    "                        proceso 'pid'.\n"
    " -f pid  --limites pid  - Muesta los límites de archivos abiertos para un\n"
    "                        proceso 'pid'.\n"
    " -t pid  --kernel pid  - stack trace acutal del proceso 'pid'.\n"
    " -j      -- cJSON      -corremos el stepa con cJSON.\n");
                         
  exit (exit_code);
}

int main(int argc, char* argv[])
{
  int opcion;
  const char* const short_options = "sjl:p:f:t:";

  const struct option long_options[] =
  {
    {     "stats",   0,  NULL, 's' },
    {     "cJSON",   0,  NULL, 'j' },
    {  "interval",   2,  NULL, 'l' },
    {       "pid",   1,  NULL, 'p' },
    {     "limites",   1,  NULL, 'f' },
    {     "kernel",   1,  NULL, 't' },

    {        NULL,   0,  NULL,  0  }
  };

  programa = argv[0];

  do
  {
    if(argc > 4) print_op(stderr,1);

    sscanf(argv[argc-1],"%d",&pid);

    opcion = getopt_long(argc, argv, short_options, long_options, NULL);

    if(argc == 1) StepA();
    else
    {
      switch (opcion)
      {
        case 's':
          StepA();
          printf("\n|---------------------------------|\n\n");
          StepB();
          break;
        case 'j':
          StepA();
          printf("\n|---------------------------|\n\n");
          StepCjson();

          break;
        case 'l':
          sscanf(argv[2], "%d", &intervalo);
          sscanf(argv[3], "%d", &duracion);

          if(intervalo > duracion)
          {

            printf("------ 'intervalo' > 'duracion' no se puede-----\n\n");
            abort();
          }
          StepA();
          printf("\n|-----------------------------|\n\n");
          StepC(intervalo , duracion);
          break;

        case 'p':
          StepA();
          printf("\n|----------------------------------|\n\n");
          StepD(pid);
          break;
        case 'f':
          StepA();
          printf("\n|--------------------|\n\n");
          limits(pid);
          break;
        case 't':
          StepA();
          printf("\n|---------------------------|\n\n");
          stack(pid);
          break;

          
        case '?':
          print_op(stderr,1);

        case -1:
          break;

        default:
          abort();
      }
      break;
    }
  } while(opcion != -1);

  return EXIT_SUCCESS;
}
