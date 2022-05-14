#include "funciones.h"

float originDistance(float u, float v)
{
  return sqrt(pow(u, 2) + pow(v, 2));
}
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

float sumatoria(float n1, float n2){
  return n1 + n2;
}

void escribirSalida(int nProcesses, char* b, float datos[nProcesses][5]){
  FILE* salida = fopen(b, "w");
  for(int i = 0; i < nProcesses; i++){
    fprintf(salida, "Disco: %i\n", i+1);
    fprintf(salida, "Media real: %f\n", datos[i][0]);
    fprintf(salida, "Media imaginaria: %f\n", datos[i][1]);
    fprintf(salida, "Potencia: %f\n", datos[i][2]);
    fprintf(salida, "Ruido: %f\n", datos[i][3]);
  }
  fclose(salida);
}