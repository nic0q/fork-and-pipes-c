// VIS recibe los elementos a traves de la lectura por los pipes. Va iterando y agregando lo que recibe a variables que parten en 0 y van
// lo que reciben, de esa forma, se obtiene la sumatoria y el iterador nos indica el N
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "funciones.h"

int main(){
  // Se definen las variables a utilizar
  float buffer[3], cumPart_r = 0, cumPart_i = 0, cumNoise = 0, potencia_total = 0, n = 0;
  while (1==1){
    // Se lee el array[3] enviado por el padre que contiene {parteReal, parteImaginaria, Ruido}
    read(STDIN_FILENO, &buffer, sizeof(float) * 3);  
    // Mensaje FIN: Si la parteReal y la parte Imaginaria son 0 significa que no hay más visibilidades que procesar       
    if (buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 0){
      // Se finaliza el calculo de las medias real e imaginaria
      cumPart_r = media(cumPart_r, n);
      cumPart_i = media(cumPart_i, n);
      // Se retorna en un array[5] = {mediaReal, mediaImaginaria, Potencia, Ruido, nVisibilidadesProc}
      float answer[5] = {cumPart_r, cumPart_i, potencia_total, cumNoise, n}; 
      // Se escribe la informacion y se manda de vuelta al padre, finalizando el proceso.
      write(STDOUT_FILENO, &answer, sizeof(float) * 5);
      return 0;
    }
    else{
      // Se realizan los calculos para las propiedades
      cumPart_r = sumatoria(cumPart_r, buffer[0]);
      cumPart_i = sumatoria(cumPart_i, buffer[1]);
      cumNoise = sumatoria(cumNoise, buffer[2]);
      potencia_total = sumatoria(potencia_total, potencia(buffer[0], buffer[1]));
      n++;
    }
  }
}