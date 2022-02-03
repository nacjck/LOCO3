#!/bin/bash

MAIN=main
#Se selecciona donde se pondran los resultados
DIRECTORIO_CSV=../../resultados/res.csv
#Donde se escriben nombres de todos los archivos en texto
ARCHIVO_IMAGENES=imagenes.txt

#ingresar al directorio del programa
cd programa/datos

make
echo "Programa creado"

touch ${ARCHIVO_IMAGENES}
for filename in ../../imagenes/*.pgm; do
    echo "$filename" >> ${ARCHIVO_IMAGENES}
done

#Se corre compresor
./${MAIN} "${ARCHIVO_IMAGENES}" "${DIRECTORIO_CSV}"

rm ${ARCHIVO_IMAGENES}
