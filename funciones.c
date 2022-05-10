#include "funciones.h"

float originDistance(float u, float v)
{
  return sqrt(pow(u, 2) + pow(v, 2));
}
float mediaReal(float part_i, int N)
{
  float res = 0;
  for (int i = 0; i < N; i++)
  {
    res += part_i;
  }
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