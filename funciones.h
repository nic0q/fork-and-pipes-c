#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

// Cabeceras de funciones
void escribirSalida(int nProcesses, char *outputFile, float datos[nProcesses][5]);
int getIndexProccess(int , int , float);
FILE* leerArchivo(char* nombreArchivo);
float originDistance(float , float );
float potencia(float , float );
float sumatoria(float , float );
float media(float , float );
