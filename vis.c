#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "funciones.h"

int main(){
  // Se definen las variables a utilizar
  float buffer[3], cumPart_r = 0, cumPart_i = 0, cumNoise = 0, potencia_total = 0, n = 0;
  while (1){
    // Se lee el array[3] enviado por el padre que contiene {parteReal, parteImaginaria, Ruido}
    read(STDIN_FILENO, &buffer, sizeof(float) * 3);  
    // Mensaje FIN: Si la parteReal y la parte Imaginaria son 0 significa que no hay más visibilidades que procesar       
    if (buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 0){
      // Se finaliza el calculo de las medias real e imaginaria
      // Se retorna en un array[5] = {mediaReal, mediaImaginaria, Potencia, Ruido, nVisibilidadesProc}
      float answer[5] = {cumPart_r/n, cumPart_i/n, potencia_total, cumNoise, n}; 
      // Se escribe la informacion y se manda de vuelta al padre, finalizando el proceso.
      write(STDOUT_FILENO, &answer, sizeof(float) * 5);
      return 0;
    }
    else{
      // Se realizan los calculos para las propiedades
      cumPart_r += buffer[0];
      cumPart_i += buffer[1];
      cumNoise += buffer[2];
      potencia_total += sqrt(pow(buffer[0], 2) + pow(buffer[1], 2));
      n++;
    }
  }
}
