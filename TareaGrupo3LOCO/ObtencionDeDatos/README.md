En esta carpeta se encuentran todos los archivos referidos a la obtención de datos del informe.
Se encuentran aquí las siguientes carpetas:

-Imagenes:
Todas las imágenes que quieran ser analizadas se deberán colocar aquí

-Programa:
El programa encargado de obtener los datos se encontrará aquí

-Resultados:
Los resultados de ejecutar los scripts se encontrarán aquí

------------------------------------------------------------------------------------------------

Además, se encuentran los siguientes scripts para realizar pruebas:

-datos.sh:
Crea un archivo .csv conteniendo la tasa de compresión de cada imagen 
ubicada en 'imagenes' en función de 's' y si se está en modo de run.
¿Cómo?
El script copia el programa principal en la carpeta 'programa/main' (por lo que
es importante asegurarse de no tener archivos pesados en la ubicación del mismo 
['../Programa']).
De aquí todos los archivos ubicados en 'modif' reemplazarán a los
archivos del programa original con el mismo nombre (por lo que si se quiere realizar
una nueva modificación a un archivo original solo hará falta agregarlo a 'modif' sin
reescribir ningún script).
Finalmente, se crea un archivo temporal conteniendo las direcciones relativas del programa
copiado a ellas, el cual es ejecutado para escribir los resultados.

-test.sh:
Comprueba que la imagen examinada sea idéntica a la comprimida y descomprimida
(incluyendo cabezal pgm). Posteriormente, se podrá encontrar en la carpeta 'resultados'
un archivo de texto que explicite si dichas imagenes "aprobaron el test".
¿Cómo?
Se comprime y descomprime cada imagen en el programa principal, donde 
se utiliza el comando 'cmp' de Linux para verificar la igualdad de
la imagen descomprimida y original.
