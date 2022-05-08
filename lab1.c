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
  pid_t pid;
  float cord_v, cord_u, sound, part_r, part_i;
  int sonNumber = 0;
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
  // VALIDACIONES
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
    // CREACION DE PIPES
    int writePipesArray[nProcesses][2];
    int readPipesArray[nProcesses][2];
  
    for (int i = 0; i < nProcesses; i++){
        pipe(writePipesArray[i]);
        pipe(readPipesArray[i]);
    }
    // CREACION DE HIJOS
    while(sonNumber < nProcesses){
      pid = fork();
      if(pid == 0){
        break;
      }
    }
    // HIJO
    if(pid == 0){
      close(writePipesArray[sonNumber][READ]); //Se cierra el lado de Lectura, ya que no leera
      dup2(writePipesArray[sonNumber][WRITE], STDOUT_FILENO); //Se genera una copia del pipe al stdout
      close(writePipesArray[sonNumber][WRITE]);

      close(readPipesArray[sonNumber][WRITE]); //Se cierra el lado de Escritura, ya que no escribira
      dup2(readPipesArray[sonNumber][READ], STDIN_FILENO); //Se genera una copia del pipe al stdout
      close(readPipesArray[sonNumber][READ]);
      //char* asd[]={"vis", "pipeflag", NULL};
      //execve("./","vis",asd);
    }
    // PADRE ONLY
    else{
    // Tendremos Nprocesses Pipes
    // lECTURA DEL ARCHIVO
    for(int i = 0; i < nProcesses; i++){
      close(readPipesArray[i][READ]);
    }

    char buffer[1024];
    float vis[6]; // array 5 size [u,v,r,i,noise]
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
      cord_u = vis[0];
      cord_v = vis[1];
      part_r = vis[2];
      part_i = vis[3];
      sound = vis[4];
      float distance = originDistance(vis[0],vis[1]);
      int index = getIndexProccess(nProcesses,discWidth,distance);
      //printf("%f %f %f %f %f %f\n",vis[0],vis[1],vis[2],vis[3],vis[4],vis[5]);
      write(readPipesArray[index][WRITE], &cord_u, sizeof(cord_u));
      write(readPipesArray[index][WRITE], &cord_v, sizeof(cord_v));
      write(readPipesArray[index][WRITE], &part_r, sizeof(part_r));
      write(readPipesArray[index][WRITE], &part_i, sizeof(part_i));
      write(readPipesArray[index][WRITE], &sound, sizeof(sound));
    }
    fclose(f);
    cord_u = 0.0;
    cord_v = 0.0;
    part_r = 0.0;
    part_i = 0.0;
    sound = 0.0;
    // FIN
    for(int i = 0; i < nProcesses; i++){
      write(readPipesArray[i][WRITE], &cord_u, sizeof(cord_u));
      write(readPipesArray[i][WRITE], &cord_v, sizeof(cord_v));
      write(readPipesArray[i][WRITE], &part_r, sizeof(part_r));
      write(readPipesArray[i][WRITE], &part_i, sizeof(part_i));
      write(readPipesArray[i][WRITE], &sound, sizeof(sound));
    }
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
}