# Makefile 
# Autor:
# 	Alexis Baladon 


all: main
.PHONY: all clean_bin entrega

#directorios
HDIR = include
CPPDIR = src
ODIR = obj

#compilador
CC = g++
CFLAGS = -Wall -Werror -I$(HDIR) -g

MODULOS = bitio coder diccionario diccionarioGrande diccionarioPequenio fabrica buffer bufferMax bufferNulo bufferNormal
#CONST_MODULES = constantes

#lista de archivos
HS = $(MODULOS:%=$(HDIR)/%.h)
CPPS = $(MODULOS:%=$(CPPDIR)/%.cpp)
OS = $(MODULOS:%=$(ODIR)/%.o)
#CONST_HS = $(CONST_MODULES:%=$(HDIR)/%.h)

MAIN=main
EJECUTABLE=main

$(ODIR)/$(MAIN).o: $(MAIN).cpp | objfolder
	$(CC) $(CCFLAGS) -c $< -o $@

$(ODIR)/%.o: $(CPPDIR)/%.cpp $(HDIR)/%.h | objfolder #$(CONST_HS) | objfolder
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

