#include "StepA.h"

#include <dirent.h>

void StepD (int pid){

DIR *descriptor;
  struct dirent *entry;
  struct stat mystats;

  int files = 0;
  char direccion[100];
  char linea[300];
  char *path;

  sprintf(direccion,"/proc/%d/fd",pid);

  descriptor = opendir(direccion);

  while( (entry=readdir(descriptor)) )
  {
    if(files >= 2) 
    {
  
      sprintf(linea,"/proc/%d/fd/%s",pid,entry->d_name);

      stat(linea, &mystats);
      //path = malloc(mystats.st_size + 1); // memoria dinamica
      path = malloc((size_t)(mystats.st_size + 1));
      
      //tipo de archivooo
      
      switch (mystats.st_mode & S_IFMT)
      {
        case S_IFBLK:  printf(" BLK "); break; // Block 
        case S_IFCHR:  printf(" CHR "); break; // Character
        case S_IFDIR:  printf(" DIR "); break; // Directory
        case S_IFIFO:  printf(" FIF "); break; // FIFO
        case S_IFLNK:  printf(" LNK "); break; // Symbolic 
        case S_IFREG:  printf(" REG "); break; // Regular f
        case S_IFSOCK: printf(" SOK "); break; // Socket 
        default:       printf(" ??? "); break; // Unknown 
      }
      readlink(linea,path,100);
      printf("%s\n", path);

      printf("\n");
    }
    files++;
  }
  closedir(descriptor);
}

void limits(int pid)
{
 
  
  char limits[45];
    sprintf(limits, "/proc/%d/limits", pid);

    FILE *limites = fopen(limits, "r");

    if (limites == NULL) {
        perror("Error al abrir el archivo /proc/[pid]/limits");
        return;
    }

    int soft, hard;
    char linea[100];  // Ajusta el tamaño según tus necesidades

    while (fgets(linea, sizeof(linea), limites) != NULL) {
        if (strstr(linea, "Max open files") != NULL) {
            sscanf(linea, "%*s %*s %d %d", &soft, &hard);
            printf("Soft limits / Hard limits: %d / %d\n", soft, hard);
            break;  // Puedes romper el bucle si encontraste la información que necesitas
        }
    }

    fclose(limites);
}


void stack(int pid)
{
  char stack[45];
  sprintf(stack,"sudo cat /proc/%d/stack",pid);

  char linea[1000];

  FILE *trace = popen(stack,"r");

  printf("%s\n", "Kernel Stack Trace:\n");

  while (fgets(linea, sizeof linea, trace) != NULL)
  {
    sscanf(linea,"%*s %s", linea);

    printf("%s\n", linea);
  }
  //fclose(trace);
  pclose(trace);
}



