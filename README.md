# Lab1_SisOp
Trabajo practico de la materia Sistemas Operativos I de la carrera Ingenieria en Computacion FCEFyN <br>
# Consigna
Archivo rdcproc.pdf <br>
# ¿Como funciona?
Para correr el programa poner en la terminal dentro del directorio donde se encuentra la carpeta <br>
```
sudo gcc -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11 main.c StepA.c StepB.c StepC.c StepD.c StepCjson.c cJSON.c -o rdproc

```
luego para seleccionar la opcion de funcionamiento por ejemplo 
```
sudo ./rdproc -l 2 100
```
primero se vera la opcion del Step A y luego al final la del Step C
