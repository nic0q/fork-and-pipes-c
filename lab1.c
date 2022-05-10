#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "funciones.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int getIndexProccess(int nProcesses, int discWidth, float distance)
{
  int j = discWidth;
  if (distance >= (nProcesses - 1) * discWidth)
  {
    return nProcesses - 1;
  }
  else
  {
    for (int i = 0; i < nProcesses; i++)
    {
      if (i * discWidth <= distance && distance < j)
      {
        return i;
      }
      j = j + discWidth;
    }
  }
}

int main(int argc, char *argv[])
{
  int nProcesses = 0, discWidth = 0;
  bool b = false;
  char *inputFile = NULL;
  char *outputFile = NULL;
  int option;
  pid_t pid;
  float cord_v, cord_u, noise, part_r, part_i;
  int IDchild = 0;
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
  // VALIDACIONES
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
  { // Si no se pudo abrir
    printf("El archivo de entrada no existe.\n");
    return 0;
  }
  else
  {
    float *estructuraFinal = malloc(sizeof(float) * nProcesses);
    for (int i = 0; i < nProcesses; i++)
    {
      estructuraFinal[i] = 0;
    }
    // CREACION DE PIPES
    int writePipesArray[nProcesses][2];
    int readPipesArray[nProcesses][2];
    int fd[2];
    float tre;
    for (int i = 0; i < nProcesses; i++)
    {
      pipe(writePipesArray[i]);
      pipe(readPipesArray[i]);
      pipe(fd);
    }
    int status;
    while (IDchild < nProcesses)
    {
      pid = fork();
      if (pid == 0)
      {
        break;
      }
      IDchild += 1;
    }
    // HIJO
    if (pid == 0)
    {
      printf("test\n");
      close(writePipesArray[IDchild][READ]);                // Se cierra el lado de Lectura, ya que no leera
      dup2(writePipesArray[IDchild][WRITE], STDOUT_FILENO); // Se genera una copia del pipe al stdout
      close(writePipesArray[IDchild][WRITE]);

      close(readPipesArray[IDchild][WRITE]);             // Se cierra el lado de Escritura, ya que no escribira
      dup2(readPipesArray[IDchild][READ], STDIN_FILENO); // Se genera una copia del pipe al stdout
      close(readPipesArray[IDchild][READ]);

      // dup2(fd[WRITE], STDOUT_FILENO);
      // close(fd[READ]); // no usamos lectura
      execl("./vis", "./vis", NULL);
    }
    // PADRE ONLY
    else
    {
      char buffer[1024];
      // Tendremos Nprocesses Pipes
      // lECTURA DEL ARCHIVO
      for (int i = 0; i < nProcesses; i++)
      {
        close(readPipesArray[i][READ]);
      }
      float vis[5]; // array 5 size [u,v,r,i,noise]
      int row = 0, col = 0;
      while (fgets(buffer, 1024, f))
      {
        col = 0;
        // Splitting the data
        char *value = strtok(buffer, ",");
        while (value)
        {
          if (col == 0)
          {
            sscanf(value, "%f", &vis[0]);
          }
          if (col == 1)
          {
            sscanf(value, "%f", &vis[1]);
          }
          if (col == 2)
          {
            sscanf(value, "%f", &vis[2]);
          }
          if (col == 3)
          {
            sscanf(value, "%f", &vis[3]);
          }
          if (col == 4)
          {
            sscanf(value, "%f", &vis[4]);
          }
          value = strtok(NULL, ",");
          col++;
          row++;
        }
        cord_u = vis[0];
        cord_v = vis[1];
        part_r = vis[2];
        part_i = vis[3];
        noise = vis[4];
        float distance = originDistance(vis[0], vis[1]);
        int index = getIndexProccess(nProcesses, discWidth, distance);
        write(readPipesArray[index][WRITE], &cord_u, sizeof(cord_u));
        write(readPipesArray[index][WRITE], &cord_v, sizeof(cord_v));
        write(readPipesArray[index][WRITE], &part_r, sizeof(part_r));
        write(readPipesArray[index][WRITE], &part_i, sizeof(part_i));
        write(readPipesArray[index][WRITE], &noise, sizeof(noise));
      }
      fclose(f);
      cord_u = 0.0;
      cord_v = 0.0;
      part_r = 0.0;
      part_i = 0.0;
      noise = 0.0;
      // FIN
      for (int i = 0; i < nProcesses; i++)
      {
        write(readPipesArray[i][WRITE], &cord_u, sizeof(cord_u));
        write(readPipesArray[i][WRITE], &cord_v, sizeof(cord_v));
        write(readPipesArray[i][WRITE], &part_r, sizeof(part_r));
        write(readPipesArray[i][WRITE], &part_i, sizeof(part_i));
        write(readPipesArray[i][WRITE], &noise, sizeof(noise));
      }
      waitpid(pid, &status, 0);
    }
    read(writePipesArray[0][READ], &tre, sizeof(float));
    printf("%f ", tre);
  }
}