// VIS recibe los elementos a traves de la lectura por los pipes. Va iterando y agregando lo que recibe a variables que parten en 0 y van
// lo que reciben, de esa forma, se obtiene la sumatoria y el iterador nos indica el N
#include <stdbool.h>
#include <unistd.h> // FILENO
#include <stdio.h>
#include <math.h>
int main()
{
  // Se leen y almacenan los datos recibidos del padre
  float buffer[3], cumPart_r = 0, cumPart_i = 0, cumNoise = 0, potencia = 0, n = 0;
  while (true)
  {
    read(STDIN_FILENO, &buffer, sizeof(float) * 3);
    if (buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 0) // Mensaje FIN: Si la parteReal y la parte Imaginaria son 0 significa que no hay más visibilidades que procesar
    {
      float mediaReal = cumPart_r / n;
      float mediaImag = cumPart_i / n;
      float answer[5] = {mediaReal, mediaImag, potencia, cumNoise, n};
      write(STDOUT_FILENO, &answer, sizeof(float) * 5);
      return 0;
    }
    else
    {
      //cumPart_r = sumatoria(cumPart_r, buffer[0])
      cumPart_r += buffer[0];
      cumPart_i += buffer[1];
      cumNoise += buffer[2];
      potencia += sqrt(pow(buffer[0], 2) + pow(buffer[1], 2));
      n++;
    }
  }
}