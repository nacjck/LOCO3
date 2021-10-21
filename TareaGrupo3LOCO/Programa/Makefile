# Makefile 
# Autores:
# 	Alexis Baladon
#	Juan Ignacio Fernandez


all: main
.PHONY: all clean_bin entrega

#directorios
HDIR = include
CPPDIR = src
ODIR = obj

#compilador
CC = gcc
CFLAGS = -Wall -Werror -I$(HDIR) -g -O3

MODULOS = compresor descompresor compartido pixelio
CONST_MODULES = modalidad

#lista de archivos
HS = $(MODULOS:%=$(HDIR)/%.h)
CPPS = $(MODULOS:%=$(CPPDIR)/%.c)
OS = $(MODULOS:%=$(ODIR)/%.o)
CONST_HS = $(CONST_MODULES:%=$(HDIR)/%.h)

MAIN=main
EJECUTABLE=main

$(ODIR)/$(MAIN).o: $(MAIN).c | objfolder
	$(CC) $(CCFLAGS) -c $< -o $@

$(ODIR)/%.o: $(CPPDIR)/%.c $(HDIR)/%.h | objfolder #$(CONST_HS) | objfolder
	$(CC) $(CCFLAGS) -c $< -o $@

$(EJECUTABLE): $(ODIR)/$(MAIN).o $(OS)
	$(CC) $(CCFLAGS) $^ -o $@
	$(info ***Compilacion exitosa***)
	$(info Archivo ejecutable generado: $(EJECUTABLE))

objfolder:
	mkdir -p $(ODIR) 

clean_bin:
	rm -f $(EJECUTABLE) $(ODIR)/$(MAIN).o $(OS)
	rmdir $(ODIR)

entrega: $(EJECUTABLE)
	tar -cvzf tarea1CDSP.tar.gz *
	$(info Archivo a entregar: tarea1CDSP.gz )

