#include "funciones.h"

float originDistance(float u, float v){
  return sqrt(pow(u,2)+pow(v,2));
}

/*
float* getArrays(float *array, int proc){
  int j = 0;
  for(int i = 0; i < proc; i++){
    
  }
}
*/
// PADRE RECIBE EL ARCHIVO DE ENTRADA, VA A LEER Y A TRAVES DE LOS PIPES LO PASA AL PROCESO HIJO
// EL HIJO VA A CALCULAR LA INFORMACION Y LUEGO EL PADRE LO MUESTRA
// 