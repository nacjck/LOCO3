#!/bin/bash

MAIN=main
#Donde se escriben nombres de todos los archivos en texto
ARCHIVO_TEST=../../resultados/test.txt
ARCHIVO_COMPRIMIDO=testEquivalencia.bin
ARCHIVO_DESCOMPRIMIDO=testEquivalencia_r.pgm
CARPETA_IMAGENES=../../imagenes

S=6

#Directorio de instalacion
cd programa/test

make

#Si habìan resultados antiguos borrarlos
if [ -f ${ARCHIVO_TEST} ]; then
    rm ${ARCHIVO_TEST}
fi

#Inicia test
touch ${ARCHIVO_TEST}
echo "Ejecutando test..."
echo ""

for filename in ${CARPETA_IMAGENES}/*.pgm; do
    for i in 0 1; do
        ./${MAIN} -c ${i} -s ${S} -m "${filename}" "${ARCHIVO_COMPRIMIDO}"
        ./${MAIN} -d -m "${ARCHIVO_COMPRIMIDO}" "${ARCHIVO_DESCOMPRIMIDO}"
        echo "RUN = ${i}" >> ${ARCHIVO_TEST}
        echo "RUN = ${i}"
        echo "$filename" >> ${ARCHIVO_TEST}
        echo "$filename"
        
        if cmp ${filename} ${ARCHIVO_DESCOMPRIMIDO}; then
            echo "Aprobado"
            echo "Aprobado" >> ${ARCHIVO_TEST}
        else
            echo "Desaprobado"
            echo "Desaprobado" >> ${ARCHIVO_TEST}
        fi
        echo ""
        echo "" >> ${ARCHIVO_TEST}
        rm $ARCHIVO_COMPRIMIDO
        rm $ARCHIVO_DESCOMPRIMIDO
    done
done
echo "Test finalizado"


