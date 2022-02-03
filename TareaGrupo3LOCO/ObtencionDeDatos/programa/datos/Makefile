# Makefile
# Autores:
# 	Alexis Baladon
#	Juan Ignacio Fernandez


all: main
.PHONY: all clean_bin entrega

#directorios
HDIR = ../../../Programa/include
CPPDIR = ../../../Programa/src
ODIR = obj

#compilador
CC = gcc
CFLAGS = -Wall -Werror -I$(HDIR) -O3

MODULOS = extractos compresor bufferCompresion imagen datosCompresion
CONST_MODULES =

#lista de archivos
HS = $(MODULOS:%=$(HDIR)/%.h)
CPPS = $(MODULOS:%=$(CPPDIR)/%.c)
OS = $(MODULOS:%=$(ODIR)/%.o)
CONST_HS = $(CONST_MODULES:%=$(HDIR)/%.h)

MAIN=main
EJECUTABLE=main

$(ODIR)/$(MAIN).o: $(MAIN).c | objfolder
	$(CC) $(CCFLAGS) -c $< -o $@

$(ODIR)/%.o: $(CPPDIR)/%.c $(HDIR)/%.h $(CONST_HS) | objfolder
	$(CC) $(CCFLAGS) -c $< -o $@

$(EJECUTABLE): $(ODIR)/$(MAIN).o $(OS)
	$(CC) $(CCFLAGS) $^ -o $@
	$(info ***Compilacion exitosa***)
	$(info Archivo ejecutable generado: $(EJECUTABLE))

objfolder:
	mkdir -p $(ODIR) 

clean_bin:
	rm -rf $(EJECUTABLE) $(ODIR)/$(MAIN).o $(OS)
	rmdir $(ODIR)

entrega: $(EJECUTABLE)
	tar -cvzf tareaATIPI.tar.gz *
	$(info Archivo a entregar: tareaATIPI.gz )
