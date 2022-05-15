#include "funciones.h"

// Entrada: Dos numeros flotantes, correspondientes a las coordenadas (u,v) de la visibilidad
// Salida: Un flotante, correspondiente a la distancia de la coordenada al origen (0,0)
// Descripcion: Funcion que calcula la distancia que hay entre unas coordenadas (u,v) al origen (0,0) en un plano
float originDistance(float u, float v){
  return sqrt(pow(u, 2) + pow(v, 2));
}

// Entrada: Dos numeros enteros, el primero la cantidad de procesos, el segundo el ancho de discos
// y un flotante, el cual es la distancia que tiene un punto al origen
// Salida: Un entero, correspondiente a que numero de disco pertenece el punto de visibilidad
// Descripcion: Funcion que a traves de la distancia de la visibilidad al origen, determina
// a que disco pertenece la visibilidad 
int getIndexProccess(int nProcesses, int discWidth, float distance){
  int j = discWidth;
  if (distance >= (nProcesses - 1) * discWidth){
    return nProcesses - 1;
  }
  else{
    for (int i = 0; i < nProcesses; i++){
      if (i * discWidth <= distance && distance < j){
        return i;
      }
      j = j + discWidth;
    }
  }
}

// Entrada: Dos flotantes, correspondientes a la parte real e imaginaria de una visibilidad
// Salida: La potencia de una visibilidad
// Descripcion: Funcion que calcula la potencia de una visibilidad
float potencia(float r, float i){
  return sqrt(pow(r, 2) + pow(i, 2));
}

// Entrada: Dos numeros flotantes
// Salida: La suma de los dos numeros flotantes de entrada
// Descripcion: Funcion que calcula la suma de dos flotantes, a largo plazo funcionando
// para calcular las sumatorias de las distintas propiedades
float sumatoria(float n1, float n2){
  return n1 + n2;
}

// Entrada: Dos numeros flotantes, donde uno es el resultado de una sumatoria y el otro
// corresponde al N de la sumatoria
// Salida: Una media (real o imaginaria dependiendo de la primera entrada)
// Descripcion: Funcion que calcula la media real o media imaginaria dividiendo la sumatoria por
// el numero de visibilidades involucradas (N)
float media (float sum, float n){
  return sum / n;
}

// Entrada: Un puntero a caracter, correspondiente al nombre del archivo a leer
// Salida: Un archivo abierto exitosamente
// Descripcion: Funcion que intenta abrir el archivo, si pudo abrirlo retorna el archivo,
// si no puede retorna 0
FILE * leerArchivo(char* nombreArchivo){
  //Se inicializa archivo como null
  FILE* archivo = NULL;
  archivo = fopen(nombreArchivo, "r"); //Lee el archivo
  if (archivo == NULL){ //Si el archivo no existe.
    printf("No se pudo abrir archivo\n");
    exit(0);
  }
  else {
    return(archivo);
  }
}

// Entrada: Un entero, el cual es el numero de discos, un caracter a puntero el cual es el
// nombre del archivo de salida y un arreglo de arreglos de flontates, que contiene todos 
// los resultados de los calculos hechos por los hijos en un solo arreglo
// Salida: Como tal no tiene salida, pero crea y escribe el archivo de salida
// Descripcion: Funcion que crea y escribe el archivo de salida, colocando la informacion
// de las propiedades asociadas a todos los discos.
void escribirSalida(int nProcesses, char *outputFile, float datos[nProcesses][5]){
  FILE *salida = fopen(outputFile, "w");
  for (int i = 0; i < nProcesses; i++){
    if (i != 0){fprintf(salida, "\n");}
      fprintf(salida, "Disco: %i\nMedia real: %f\nMedia imaginaria: %f\nPotencia: %f\nRuido: %f",
      i + 1,datos[i][0],datos[i][1],datos[i][2],datos[i][3]);
  }
  fclose(salida);
}