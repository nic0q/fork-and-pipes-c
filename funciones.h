#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

// Cabeceras de funciones
void escribirSalida(int nProcesses, char *outputFile, float datos[nProcesses][5]);
int getIndexProccess(int nProcesses, int discWidth, float distance);
FILE* leerArchivo(char* nombreArchivo);
float originDistance(float u, float v);
float potencia(float r, float i);
float sumatoria(float n1, float n2);
float media(float sum, float n);
