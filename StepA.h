#ifndef Stepa_h
#define Stepa_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "cJSON.h"
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

void StepA();
void StepB();
void StepC(int intervalo,int duracion );
void StepD(int pid);
void StepCjson();
void limits(int pid);
void stack(int pid);



#endif

