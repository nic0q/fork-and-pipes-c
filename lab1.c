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
  // Se definen las variables a utilizar
  int nProcesses = 0, discWidth = 0, option, IDchild = 0, b = 0;
  char *inputFile = NULL, *outputFile = NULL;
  pid_t pid;
  while ((option = getopt(argc, argv, "i:o:n:d:b")) != -1){
    switch (option){
    // Nombre del archivo de entrada
    case 'i':
      inputFile = optarg;
      break;
    // Nombre del archivo de salida  
    case 'o':
      outputFile = optarg;
      break;
    // Numero de discos
    case 'n':
      sscanf(optarg, "%d", &nProcesses);
      break;
    // Ancho de cada disco
    case 'd':
      sscanf(optarg, "%d", &discWidth);
      break;
    // Bandera | flag | opcional
    case 'b':
      b = 1;
      break;
    case '?':
      exit(0);
    // En caso que no se haya ingresado alguna flag obligatoria, se aborta la ejecucion
    default:
      abort();    
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
  // Creacion de Pipes, se crean arreglos de pipes de escritura y lectura. En palabras simples
  // cada hijo tendra dos pipes. Uno donde el hijo leera los datos enviados por el padre y 
  // otro donde el hijo escribira los calculos realizados para que puedan ser leidos por el padre
  for (int i = 0; i < nProcesses; i++){
    pipe(writePipesArray[i]);
    pipe(readPipesArray[i]);
  }
  // Creacion de Hijos, cada hijo tiene un PID y un ID asociado. El ID permite saber que
  // pipes se le deben asignar especificamente
  while (IDchild < nProcesses){
    pid = fork();
    // Se guardan los PIDs en un arreglo, esto para no perderlo y para mostrarlo si la flag (b)
    // esta activada
    arrPids[IDchild] = pid;
    if (pid == 0){
      break;
    }
    IDchild += 1;
  }
  // Proceso HIJO
  if (pid == 0){
    // printf("Hijo ID: %d\n", IDchild);                  // for debugging
    // Se cierra el lado de Lectura, ya que no leera
    close(writePipesArray[IDchild][READ]); 
    // Se genera una copia del pipe al STDOUT              
    dup2(writePipesArray[IDchild][WRITE], STDOUT_FILENO);
    // Debido a lo anterior se cierra el pipe de escritura
    close(writePipesArray[IDchild][WRITE]);

    // Se cierra el lado de Escritura, ya que no escribira
    close(readPipesArray[IDchild][WRITE]);
    // Se genera una copia del pipe al STDOUT
    dup2(readPipesArray[IDchild][READ], STDIN_FILENO);
    // Debido a lo anterior se cierra el pipe de escritura
    close(readPipesArray[IDchild][READ]);

    // Se utiliza execl para llamar al archivo vis
    execl("./vis", "./vis", NULL);
    // Si lo anterior falla, los hijos finalizan su proceso prematuramente
    perror("error execl");
    exit(EXIT_FAILURE);
  }

  // Proceso PADRE
  else{
    float u, v, vis[3], listaSol[5], arrayMaster[nProcesses][5];
    // Mensaje para detenerse
    float stopMessage[3] = {0, 0, 0};
    int row = 0, col = 0;
    char buffer[1000];
    // Se cierran todos los lados de lectura de los pipes de lectura
    for (int i = 0; i < nProcesses; i++){
      close(readPipesArray[i][READ]);
    }
    // Se lee el archivo de entrada
    while (fgets(buffer, 1000, f) != NULL){
      // Se guarda un array de tamano 3 [r,i,noise]
      sscanf(buffer, "%f,%f,%f,%f,%f", &u, &v, &vis[0], &vis[1], &vis[2]);
      // Se calcula la distancia al origen de cada visibilidad
      float oDistance = originDistance(u, v);
      // Se obtiene a que disco le pertenece la visibilidad
      int index = getIndexProccess(nProcesses, discWidth, oDistance);
      // Se envia la informacion por el pipe correspondiente hacia al hijo
      write(readPipesArray[index][WRITE], &vis, sizeof(float) * 3);
    }
    // Se cierra el archivo de lectura
    fclose(f);
    // Se envia el mensaje de FIN a cada proceso hijo
    for (int i = 0; i < nProcesses; i++){
      write(readPipesArray[i][WRITE], &stopMessage, sizeof(float) * 3);
    }
    for (int i = 0; i < nProcesses; i++){
      // Se lee la informacion enviada por el hijo y se guarda en un array de nombre
      // listaSol [media_real,media_imaginaria,potencia,ruido,numero_visibilidades]
      read(writePipesArray[i][READ], &listaSol, sizeof(float) * 5);
      // Parse para cambiar de float a int
      int nVis = listaSol[4];
      if (b == 1){
        // Si la flag esta activada se muestra informacion adicional
        printf("Soy el hijo de pid %d, procesé %d visibilidades\n", arrPids[i], nVis);
      }
      // Se guarda la informacion de listaSol en otro arreglo que contendra todas las
      // listaSol de todos los hijos, es decir, un arrreglo donde van a estar todos los
      // resultados de los calculos de los hijos
      arrayMaster[i][0] = listaSol[0];
      arrayMaster[i][1] = listaSol[1];
      arrayMaster[i][2] = listaSol[2];
      arrayMaster[i][3] = listaSol[3];
      arrayMaster[i][4] = listaSol[4];
    }
    // Utilizando el arreglo anterior, se procede a crear y escribir el archivo de salida
    escribirSalida(nProcesses, outputFile, arrayMaster);
    // Se finaliza el programa.
    return 0;
  }
}