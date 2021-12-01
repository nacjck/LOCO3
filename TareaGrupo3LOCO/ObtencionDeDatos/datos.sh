#!/bin/bash

MAIN=compresorLOCO

#limpiar carpetas
rm -rf programa/main/*
rm -rf resultados/*
echo "Archivos antiguos borrados"

#copiar programas originales
cp -r  ../Programa/. programa/main
echo "Archivos de programa copiados"

#sobreescribe archivos modificados en programa

#src
for filename in programa/modif/*.c; do
    if [ "$filename"!="programa/modif/${MAIN}.c" ]; then
        cp "$filename" programa/main/src
    fi
done

#main
if test -f programa/modif/${MAIN}.c; then
    cp programa/modif/${MAIN}.c programa/main
fi

#include
for filename in programa/modif/*.h; do
    cp "$filename" programa/main/include
done
echo "Archivos de prueba sustituidos"

#ingresar a la carpeta del programa
cd programa/main

#hacer make
bash make.sh
echo "Programa compilado"

#Se escriben nombres de todos los archivos en texto
ARCHIVO_IMAGENES=imagenes.txt
touch ${ARCHIVO_IMAGENES}
for filename in ../../imagenes/*.pgm; do
    echo "$filename" >> ${ARCHIVO_IMAGENES}
done

#Se selecciona donde se pondran los resultados
DIRECTORIO_CSV=../../resultados/res.csv

#Se corre compresor
./compresorLOCO "${ARCHIVO_IMAGENES}" "${DIRECTORIO_CSV}"
