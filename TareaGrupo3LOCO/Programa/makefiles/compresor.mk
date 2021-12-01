# Makefile 
# Autores:
# 	Alexis Baladon
#	Juan Ignacio Fernandez


all: compresorLOCO
.PHONY: all clean_bin

#directorios
HDIR = ../include
CPPDIR = ../src
ODIR = ../obj/compresor

#compilador
CC = gcc
CFLAGS = -Wall -Werror -I$(HDIR) -g -O3

MODULOS = compartido compresor compresorIO pixelio
CONST_MODULES = modalidad

#lista de archivos
HS = $(MODULOS:%=$(HDIR)/%.h)
CPPS = $(MODULOS:%=$(CPPDIR)/%.c)
OS = $(MODULOS:%=$(ODIR)/%.o)
CONST_HS = $(CONST_MODULES:%=$(HDIR)/%.h)

MAIN=compresorLOCO
EJECUTABLE=compresorLOCO

$(ODIR)/$(MAIN).o: ../$(MAIN).c | objfolder
	$(CC) $(CCFLAGS) -c $< -o $@

$(ODIR)/%.o: $(CPPDIR)/%.c $(HDIR)/%.h $(CONST_HS) | objfolder
	$(CC) $(CCFLAGS) -c $< -o $@

$(EJECUTABLE): $(ODIR)/$(MAIN).o $(OS)
	$(CC) $(CCFLAGS) $^ -o $@
	$(info ***Compilacion de compresor exitosa***)
	$(info Archivo ejecutable generado: $(EJECUTABLE))

objfolder:
	mkdir -p $(ODIR) 

clean_bin:
	rm -f $(EJECUTABLE) $(ODIR)/$(MAIN).o $(OS)
