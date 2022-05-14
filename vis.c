// VIS recibe los elementos a traves de la lectura por los pipes. Va iterando y agregando lo que recibe a variables que parten en 0 y van
// lo que reciben, de esa forma, se obtiene la sumatoria y el iterador nos indica el N
#include <unistd.h>
#include <stdio.h>
#include <math.h>
int main()
{
  // Se leen y almacenan los datos recibidos del padre
  float buffer[3], cumPart_r = 0, cumPart_i = 0, cumNoise = 0, potencia = 0, n = 0;
  while (1==1){
    read(STDIN_FILENO, &buffer, sizeof(float) * 3);         // Se lee el array[3] enviado por el padre que contiene {parteReal, parteImaginaria, Ruido}
    if (buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 0){ // Mensaje FIN: Si la parteReal y la parte Imaginaria son 0 significa que no hay más visibilidades que procesar
      float answer[5] = {cumPart_r / n, cumPart_i / n, potencia, cumNoise, n}; // Se retorna en un array[5] = {mediaReal, mediaImaginaria, Potencia, Ruido, nVisibilidadesProc}
      write(STDOUT_FILENO, &answer, sizeof(float) * 5);
      return 0;
    }
    else{
      // cumPart_r = sumatoria(cumPart_r, buffer[0])  BORRAR SI NO SE OCUPARA UWU?
      cumPart_r += buffer[0];
      cumPart_i += buffer[1];
      cumNoise += buffer[2];
      potencia += sqrt(pow(buffer[0], 2) + pow(buffer[1], 2));
      n++;
    }
  }
}