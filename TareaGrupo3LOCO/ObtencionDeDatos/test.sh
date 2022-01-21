#!/bin/bash

MAIN=main
#Donde se escriben nombres de todos los archivos en texto
ARCHIVO_TEST=../ObtencionDeDatos/resultados/test.txt
ARCHIVO_COMPRIMIDO=testEquivalencia.bin
ARCHIVO_DESCOMPRIMIDO=testEquivalencia_r.pgm
CARPETA_IMAGENES=../ObtencionDeDatos/imagenes

S=6

cd ../Programa

make

if [ -f ${ARCHIVO_TEST} ]; then
    rm ${ARCHIVO_TEST}
fi

touch ${ARCHIVO_TEST}
echo "Ejecutando test..."
for filename in ${CARPETA_IMAGENES}/*.pgm; do
    for i in 0 1; do
        ./${MAIN} -c ${i} -s ${S} -m "${filename}" "${ARCHIVO_COMPRIMIDO}"
        ./${MAIN} -d -m "${ARCHIVO_COMPRIMIDO}" "${ARCHIVO_DESCOMPRIMIDO}"
        echo "RUN = ${i}" >> ${ARCHIVO_TEST}
        echo "$filename" >> ${ARCHIVO_TEST}
        if cmp ${filename} ${ARCHIVO_DESCOMPRIMIDO}; then
            echo "Aprobado" >> ${ARCHIVO_TEST}
        else
            echo "Desaprobado" >> ${ARCHIVO_TEST}
        fi
        echo "" >> ${ARCHIVO_TEST}
        rm $ARCHIVO_COMPRIMIDO
        rm $ARCHIVO_DESCOMPRIMIDO
    done
done
echo "Test finalizado"


