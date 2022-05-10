// VIS recibe los elementos a traves de la lectura por los pipes. Va iterando y agregando lo que recibe a variables que parten en 0 y van
// lo que reciben, de esa forma, se obtiene la sumatoria y el iterador nos indica el N
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
  // Se leen y almacenan los datos recibidos del padre
  float cumU = 0, cumV = 0, cumPart_r = 0, cumPart_i = 0, cumNoise = 0;
  float tempArr[100], u, v, part_r, part_i, noise;
  float n = 0;
  int i = 0;
  read(STDIN_FILENO, &u, sizeof(float));
  read(STDIN_FILENO, &v, sizeof(float));
  read(STDIN_FILENO, &part_r, sizeof(float));
  read(STDIN_FILENO, &part_i, sizeof(float));
  read(STDIN_FILENO, &noise, sizeof(float));
  write(STDOUT_FILENO, &u, sizeof(float));
  cumU += u;
  cumV += v;
  cumPart_r += part_r;
  cumPart_i += part_i;
  cumNoise += noise;
  return 0;
}