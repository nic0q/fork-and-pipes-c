#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include "funciones.h"
#define WRITE 1
#define READ 0

int main(int argc, char *argv[]){
  int nProcesses = 0, discWidth = 0, option, IDchild = 0, b = 0;
  char *inputFile = NULL, *outputFile = NULL;
  pid_t pid;
  while ((option = getopt(argc, argv, "i:o:n:d:b")) != -1){
    switch (option){
    case 'i': // nombre del archivo de entrada
      inputFile = optarg;
      break;
    case 'o': // nombre del archivo de salida
      printf("outputFile: %s\n", optarg);
      outputFile = optarg;
      break;
    case 'n': // numero de discos
      sscanf(optarg, "%d", &nProcesses);
      break;
    case 'd': // ancho de cada disco
      sscanf(optarg, "%d", &discWidth);
      break;
    case 'b': // bandera | flag | opcional
      b = 1;
      break;
    case '?':
      break;
    }
  }
  // Validaciones / Lectura del archivo
  FILE *f = leerArchivo(inputFile);
  if (nProcesses <= 0){
    printf("Ingrese una cantidad de discos valida\n");
    return 0;
  }
  if (discWidth <= 0){
    printf("Ingrese un ancho de disco valido\n");
    return 0;
  }
  int readPipesArray[nProcesses][2], writePipesArray[nProcesses][2], arrPids[nProcesses], status;
  for (int i = 0; i < nProcesses; i++){ // Creacion de Pipes
    pipe(writePipesArray[i]);
    pipe(readPipesArray[i]);
  }
  while (IDchild < nProcesses){ // Creacion de Hijos
    pid = fork();
    arrPids[IDchild] = pid;
    if (pid == 0){
      break;
    }
    IDchild += 1;
  }
  if (pid == 0){ // Proceso HIJO
    // printf("Hijo ID: %d\n", IDchild);                  // for debugging
    close(writePipesArray[IDchild][READ]);                // Se cierra el lado de Lectura, ya que no leera
    dup2(writePipesArray[IDchild][WRITE], STDOUT_FILENO); // Se genera una copia del pipe al stdout
    close(writePipesArray[IDchild][WRITE]);

    close(readPipesArray[IDchild][WRITE]);             // Se cierra el lado de Escritura, ya que no escribira
    dup2(readPipesArray[IDchild][READ], STDIN_FILENO); // Se genera una copia del pipe al stdout
    close(readPipesArray[IDchild][READ]);

    execl("./vis", "./vis", NULL);
    perror("error execl");
    exit(EXIT_FAILURE);
  }
  else{ // Proceso PADRE
    float u, v, vis[3], listaSol[5], arrayMaster[nProcesses][5];
    float stopMessage[3] = {0, 0, 0}; // Mensaje para detenerse
    int row = 0, col = 0;
    char buffer[1000];
    for (int i = 0; i < nProcesses; i++){
      close(readPipesArray[i][READ]);
    }
    while (fgets(buffer, 1000, f) != NULL){
      sscanf(buffer, "%f,%f,%f,%f,%f", &u, &v, &vis[0], &vis[1], &vis[2]);   // array 3 size [r,i,noise]
      float oDistance = originDistance(u, v);
      int index = getIndexProccess(nProcesses, discWidth, oDistance);
      write(readPipesArray[index][WRITE], &vis, sizeof(float) * 3);
    }
    fclose(f);
    for (int i = 0; i < nProcesses; i++){
      write(readPipesArray[i][WRITE], &stopMessage, sizeof(float) * 3);
    }
    for (int i = 0; i < nProcesses; i++){
      read(writePipesArray[i][READ], &listaSol, sizeof(float) * 5);
      int nVis = listaSol[4]; // Parse para cambiar de float a int
      if (b == 1){
        printf("Soy el hijo de pid %d, procesé %d visibilidades\n", arrPids[i], nVis);
      }
      arrayMaster[i][0] = listaSol[0];
      arrayMaster[i][1] = listaSol[1];
      arrayMaster[i][2] = listaSol[2];
      arrayMaster[i][3] = listaSol[3];
      arrayMaster[i][4] = listaSol[4];
    }
    escribirSalida(nProcesses, outputFile, arrayMaster);
  }
}