#!/bin/bash

MAIN=main
#Se selecciona donde se pondran los resultados
DIRECTORIO_CSV=../../resultados/res.csv
#Donde se escriben nombres de todos los archivos en texto
ARCHIVO_IMAGENES=imagenes.txt

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
    if [ $filename != programa/modif/*.h ]; then
        cp "$filename" programa/main/include
    fi
done
echo "Archivos de prueba sustituidos"

#ingresar a la carpeta del programa
cd programa/main

#hacer make
make
echo "Programa creado"

touch ${ARCHIVO_IMAGENES}
for filename in ../../imagenes/*.pgm; do
    echo "$filename" >> ${ARCHIVO_IMAGENES}
done

#Se corre compresor
./${MAIN} "${ARCHIVO_IMAGENES}" "${DIRECTORIO_CSV}"

#Se borran archivos temporales
rm -r ./*
