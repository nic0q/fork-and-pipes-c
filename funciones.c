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

void escribirSalida(int i, float mr, float mi, float pot, float ruido, char* b){
  FILE* salida = fopen(b, "a");
  fprintf(salida, "Disco: %i\n", i);
  fprintf(salida, "Medial real: %f\n", mr);
  fprintf(salida, "Media imaginaria: %f\n", mi);
  fprintf(salida, "Potencia: %f\n", pot);
  fprintf(salida, "Ruido total: %f\n", ruido);
}