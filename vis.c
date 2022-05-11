// VIS recibe los elementos a traves de la lectura por los pipes. Va iterando y agregando lo que recibe a variables que parten en 0 y van
// lo que reciben, de esa forma, se obtiene la sumatoria y el iterador nos indica el N
#include <stdio.h>
#include <unistd.h> // FILENO
#include <stdbool.h>

int main()
{
  // Se leen y almacenan los datos recibidos del padre
  float buffer[3], cumPart_r = 0, cumPart_i = 0, cumNoise = 0, n = 0;
  while (true)
  {
    read(STDIN_FILENO, &buffer, sizeof(float) * 3);
    if (buffer[0] == 0)
    {
      float mediaReal = cumPart_r / n;
      float mediaImag = cumPart_i / n;
      float totalNoise = cumNoise;
      float answer[5] = {mediaReal, mediaImag, n, totalNoise, n};
      write(STDOUT_FILENO, &answer, sizeof(float) * 5);
      return 0;
    }
    else
    {
      cumPart_r += buffer[0];
      cumPart_i += buffer[1];
      cumNoise += buffer[2];
      n++;
    }
  }
}