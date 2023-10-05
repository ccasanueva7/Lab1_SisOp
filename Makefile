# Nombre del ejecutable
TARGET = lab1

# Archivos fuente
SRCS = main.c StepA.c StepB.c StepC.c StepD.c StepCjson.c cJSON.c

# Objetos generados a partir de los archivos fuente
OBJS = $(SRCS:.c=.o)

# Flags del compilador
CFLAGS = -Wall -I/path/to/cjson/headers 

# Regla predeterminada 
all: $(TARGET)

# Regla para construir el programa
$(TARGET): $(OBJS)
	gcc $(CFLAGS) -o $(TARGET) $(OBJS)

# Regla para compilar
%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

# Limpia los archivos objeto y el programa compilado
clean:
	rm -f $(OBJS) $(TARGET)
