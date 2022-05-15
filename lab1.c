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
  int n = 0, discWidth = 0, option, IDchild = 0, b = 0;
  char *inputFile = NULL, *outputFile = NULL;
  pid_t pid;
  while ((option = getopt(argc, argv, "i:o:n:d:b")) != -1){
    switch (option){
    case 'i': // nombre del archivo de entrada
      inputFile = optarg;
      break;
    case 'o': // nombre del archivo de salida
      outputFile = optarg;
      break;
    case 'n': // numero de discos
      sscanf(optarg, "%d", &n);
      break;
    case 'd': // ancho de cada disco
      sscanf(optarg, "%d", &discWidth);
      break;
    case 'b': // bandera | flag | opcional
      b = 1;
      break;
    case '?':
      exit(0);
    default: //En caso que no se haya ingresado alguna flag obligatoria, se aborta la ejecucion
      abort();    
    }
  }
  // Validaciones / Lectura del archivo
  FILE *f = leerArchivo(inputFile);
  if (n <= 0){ printf("Ingrese una cantidad de discos valida\n"); exit(0);}
  if (discWidth <= 0){ printf("Ingrese un ancho de disco valido\n"); exit(0);}

  int readPipesArray[n][2], writePipesArray[n][2], arrPids[n], status;
  for (int i = 0; i < n; i++){ // Creacion de Pipes
    pipe(writePipesArray[i]);
    pipe(readPipesArray[i]);
  }
  while (IDchild < n){ // Creacion de Hijos
    pid = fork();
    arrPids[IDchild] = pid;
    if (pid == 0){
      break;
    }
    IDchild += 1;
  }
  if (pid == 0){ // Proceso HIJO
    close(writePipesArray[IDchild][READ]);                // Se cierra el lado de Lectura, ya que no leera
    dup2(writePipesArray[IDchild][WRITE], STDOUT_FILENO); // Se genera una copia del pipe al stdout
    close(writePipesArray[IDchild][WRITE]);

    close(readPipesArray[IDchild][WRITE]);             // Se cierra el lado de Escritura, ya que no escribira
    dup2(readPipesArray[IDchild][READ], STDIN_FILENO); // Se genera una copia del pipe al stdout
    close(readPipesArray[IDchild][READ]);

    execl("vis", "vis", NULL);
    perror("error execl");
    exit(EXIT_FAILURE);
  }
  else{ // Proceso PADRE
    float u, v, vis[3], listaSol[5], arrayMaster[n][5], stopMessage[3] = {0, 0, 0};
    int row = 0, col = 0;
    char buffer[1000];
    for (int i = 0; i < n; i++){
      close(readPipesArray[i][READ]);
    }
    while (fgets(buffer, 1000, f) != NULL){
      sscanf(buffer, "%f,%f,%f,%f,%f", &u, &v, &vis[0], &vis[1], &vis[2]);   // array 3 size [r,i,noise]
      float oDistance = originDistance(u, v);
      int index = getIndexProccess(n, discWidth, oDistance);
      write(readPipesArray[index][WRITE], &vis, sizeof(float) * 3);
    }
    fclose(f);
    for (int i = 0; i < n; i++){  // Se envia mensaje de FIN
      write(readPipesArray[i][WRITE], &stopMessage, sizeof(float) * 3);
    }
    for (int i = 0; i < n; i++){  // Se recibe la lista solución
      read(writePipesArray[i][READ], &listaSol, sizeof(float) * 5);
      int nVis = listaSol[4]; // Parse para cambiar de float a int
      if (b == 1){
        printf("Soy el hijo de pid %d, procesé %d visibilidades\n", arrPids[i], nVis);
      }
      for(int j = 0; j < 5;j++){
        arrayMaster[i][j] = listaSol[j];
      }
    }
    escribirSalida(n, outputFile, arrayMaster);
  }
}