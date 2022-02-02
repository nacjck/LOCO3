 Para correr el archivo 'main.c' desde la consola, usar:  
  ./main -d inputFile outputFile (al descomprimir)  
  ./main -c [modalidad] [-s valorS] [-m] inputFile outputFile  (al comprimir)
 
 (Valores encerrados entre '[]' son opcionales)

 Donde:  
   -c <number> habilita la compresión,  
   si recibe 1 comprime en modo de run,  
   si recibe un 0 o nada lo hace en modo normal.  
   -d habilita la descompresión  
   -s <number>: valor del parámetro s 
   si no hay parámetro se tomara el valor DEFAULT_S  
   -m silencia el output de la consola (excepto para errores)  
   inputFile: archivo de entrada  
   outputFile: archivo de salida
   
Ejemplos:  
    ./main -c 1 -s 10 "imagen.pgm" "imagen.bin"  
    ./main -d "imagen.bin" "imagen.pgm"  
