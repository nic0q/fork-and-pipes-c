#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

main(int argc, char *argv[]) {
   int n, d;
   bool b = false;
   char *inputFile = NULL;
   char *outputFile = NULL;
   int option;
   while((option = getopt(argc, argv, "i:o:n:d:b")) != -1){ //get option from the getopt() method
      switch(option){
        case 'i': // nombre del archivo de entrada
           printf("inputFile: %s\n", optarg);
           inputFile = optarg;
           break;
        case 'o': // nombre del archivo de salida
           printf("outputFile: %s\n", optarg);
           outputFile = optarg;
           break;
        case 'n': // numero de discos
            printf("n: %s\n", optarg);
            sscanf(optarg, "%d", &n);
           break;
        case 'd': // ancho de cada disco
           printf("d: %s\n", optarg);
           sscanf(optarg, "%d", &d);
           break;
        case 'b': // bandera | flag | opcional
           b = true;
           break;                                  
        case '?':
           printf("unknown option: %c\n", optopt);
           break;
      }
   }
   if(n <= 0){
      printf("cantidad de discos <= 0");
   }
   if(d <= 0){
      printf("ancho de disco <= 0");
   }
   //Se verifica que el archivo de entrada exista
   FILE *f = fopen(inputFile,"r");
   if(f == NULL){ //Si no se pudo abrir
     printf("El archivo de entrada no existe.\n");
   }
   printf("mi f %d",&f);
   fclose(f); //Se cierra el archivo de entrada
   //Verificar que la cantidad de lineas sea valida
}