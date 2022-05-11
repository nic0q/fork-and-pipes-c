#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include "funciones.h" // Dato perturbador, si se coloca el "funciones.h" antes de los include <> tira error
#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
  int nProcesses = 0, discWidth = 0, option, IDchild = 0;
  char *inputFile = NULL, *outputFile = NULL;
  bool b = false;
  float listaSol[5];
  pid_t pid;
  while ((option = getopt(argc, argv, "i:o:n:d:b")) != -1)
  { // get option from the getopt() method
    switch (option)
    {
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
  // Validaciones
  if (nProcesses <= 0)
  {
    printf("Ingrese una cantidad de discos valida\n");
    return 0;
  }
  if (discWidth <= 0)
  {
    printf("Ingrese un ancho de disco valido\n");
    return 0;
  }
  FILE *f = fopen(inputFile, "r");
  if (f == NULL)
  { // Si no se pudo abrir el archivo de entrada
    printf("El archivo de entrada no existe.\n");
    return 0;
  }
  else
  {
    float *estructuraFinal = malloc(sizeof(float) * nProcesses);
    int readPipesArray[nProcesses][2], writePipesArray[nProcesses][2], status;
    // Creacion de Pipes
    for (int i = 0; i < nProcesses; i++)
    {
      pipe(writePipesArray[i]);
      pipe(readPipesArray[i]);
    }
    // Arreglo de resultados
    // Reserva de memoria para el arreglo de resultados
    while (IDchild < nProcesses)
    {
      pid = fork();
      if (pid == 0)
      {
        break;
      }
      IDchild += 1;
    }
    if (pid == 0) // Proceso HIJO
    {
      // printf("Hijo ID: %d\n", IDchild);                     // for debugging
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
    else // Proceso PADRE
    {
      char buffer[10000];
      float u, v, vis[3]; // array 5 size [r,i,noise]
      int row = 0, col = 0;
      for (int i = 0; i < nProcesses; i++)
      {
        close(readPipesArray[i][READ]);
      }
      while (fgets(buffer, 500, f) != NULL)
      {
        sscanf(buffer, "%f,%f,%f,%f,%f", &u, &v, &vis[0], &vis[1], &vis[2]);
        float oDistance = originDistance(u, v);
        int index = getIndexProccess(nProcesses, discWidth, oDistance);
        write(readPipesArray[index][WRITE], &vis, sizeof(float) * 3);
      }
    }
    fclose(f);
    float stop[3];
    stop[0] = 0; // mensaje de detenerse
    for (int i = 0; i < nProcesses; i++)
    {
      write(readPipesArray[i][WRITE], &stop, sizeof(float) * 3);
    }
    for (int i = 0; i < nProcesses; i++)
    {
      read(writePipesArray[i][READ], &listaSol, sizeof(float) * 5); // se leen los resultados de cada hijo
      printf("mediaR: %f mediaI: %f potencia: %f ruido: %f Nvisibilidades: %f", listaSol[0], listaSol[1], listaSol[2], listaSol[3], listaSol[4]);
      printf("\n");
    }
  }
}