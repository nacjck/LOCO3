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

Dentro de la carpeta "programa" se encuentran los archivos del programa original 
que fue necesario modificar para obtener los datos.  

El script se encarga de utizar estos archivos junto a los del programa original para que puedan ser ejecutados.  
Por último, se crea un archivo temporal conteniendo las direcciones relativas del programa
copiado a ellas, el cual es ejecutado para escribir los resultados en la dirección especificada.  

-test.sh:
Comprueba que las imágenes examinadas sean idénticas a las comprimidas y descomprimidas
(incluyendo cabezal pgm). Posteriormente, se podrá encontrar en la carpeta 'resultados'
un archivo de texto que explicite si dichas imagenes "aprobaron el test".

¿Cómo?

Dentro de la carpeta "programa" se encuentran los archivos del programa original 
que fue necesario modificar para obtener los datos.  

El script se encarga de utizar estos archivos junto a los del programa original para que puedan ser ejecutados.  
Por último, se comprime y descomprime cada imagen en el programa principal, donde 
se utiliza el comando 'cmp' de Linux para verificar la igualdad de
la imagen descomprimida y original.
