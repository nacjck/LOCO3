#!/bin/bash

MAIN=main
#Donde se escriben nombres de todos los archivos en texto
ARCHIVO_TEST=../../resultados/test.txt
ARCHIVO_COMPRIMIDO=testEquivalencia.bin
ARCHIVO_DESCOMPRIMIDO=testEquivalencia_r.pgm
CARPETA_IMAGENES=../ObtencionDeDatos/imagenes

S=6

cd ../Programa

make

if [ -f ${ARCHIVO_TEST} ]
    rm ${ARCHIVO_TEST}
fi

touch ${ARCHIVO_TEST}
for filename in ${CARPETA_IMAGENES}/*.pgm; do
    for i in 0 1; do
        ./${MAIN} -c ${i} -s ${S} "${filename}" "${ARCHIVO_COMPRIMIDO}"
        ./${MAIN} -d "${ARCHIVO_COMPRIMIDO}" "${ARCHIVO_DESCOMPRIMIDO}"
        echo "$filename" >> ${ARCHIVO_IMAGENES}
        if [ cmp ${filename} ${ARCHIVO_DESCOMPRIMIDO} ]; then
            echo ${ARCHIVO_TEST} >> "Aprobado"
        else
            echo ${ARCHIVO_TEST} >> "Desaprobado"
        fi
    done
done

