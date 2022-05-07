#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "funciones.h"
#include <stdlib.h>
#define READ 0
#define WRITE 1

int getIndexProccess(int nProcesses, int discWidth, float distance){
  int j = discWidth;
  if(distance >= (nProcesses - 1 ) * discWidth){
    return nProcesses-1;
  }
  else{
    for(int i = 0; i < nProcesses; i++){
      if(i*discWidth <= distance && distance < j){
        return i;
      }
      j = j + discWidth;
    }
  }
}

int main(int argc, char *argv[]) {
  int nProcesses = 0, discWidth = 0;
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
        sscanf(optarg, "%d", &nProcesses);
        break;
      case 'd': // ancho de cada disco
        printf("d: %s\n", optarg);
        sscanf(optarg, "%d", &discWidth);
        break;
      case 'b': // bandera | flag | opcional
        b = true;
        break;                                  
      case '?':
        printf("unknown option: %c\n", optopt);
        break;
     }
  }
  if(nProcesses <= 0){
     printf("Ingrese una cantidad de discos valida\n");
     return 0;
  }
  if(discWidth <= 0){
     printf("Ingrese un ancho de disco valido\n");
     return 0;
  }
  FILE *f=fopen(inputFile,"r");
  if(f == NULL){ //Si no se pudo abrir
    printf("El archivo de entrada no existe.\n");
    return 0;
  }
  else{
    // create the array of arrays
    float **m = (float**)malloc(nProcesses * sizeof(float*)); // dynamic array "nProcess" size
    for(int i = 0; i < nProcesses; i++){
      m[i] = (float*)malloc(5 * sizeof(float)); // array 5 size [u,v,r,i,noise]
    }
    char buffer[1024];
    float vis[5]; // array 5 size [u,v,r,i,noise]
    int row = 0,col = 0;
    while (fgets(buffer,1024,f)) {
      col = 0;
      // Splitting the data
      char* value = strtok(buffer,",");
      while (value) {
        if (col == 0){
          sscanf(value,"%f",&vis[0]);
        }
        if (col == 1) {
          sscanf(value,"%f",&vis[1]);
        }
        if (col == 2) {
          sscanf(value,"%f",&vis[2]);
        }
        if (col == 3) {
          sscanf(value,"%f",&vis[3]);
        }
        if (col == 4) {
          sscanf(value,"%f",&vis[4]);
        }
        value = strtok(NULL, ",");
        col++;
        row++;

      }
      float distance = originDistance(vis[0],vis[1]);
      int index = getIndexProccess(nProcesses,discWidth,distance);
      printf("%f %d\n",distance,index);
    }
    fclose(f);
  }
  // 4 discos - 100 por cada disco
  // Crea cada proceso hijo con un rango determinado
  // int rangomin = 0, rangomax = 0;
  // int pid = 10;
  // for(int i = 0;i < n;i++){
  //    pid = fork();
  //    if(pid == 0){  // Si es el hijo
  //      rangomin +=d;
  //      rangomax +=d-1;
  //      printf("a %d %d %d \n",pid,rangomin,rangomax);
  //   }
  //   else{  // Si es el padre

  //   }
  // }
}
