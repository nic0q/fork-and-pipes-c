#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void escribirSalida(int nProcesses, char *b, float datos[nProcesses][5]);
int getIndexProccess(int nProcesses, int discWidth, float distance);
FILE* leerArchivo(char* nombreArchivo);
float originDistance(float u, float v);
float sumatoria(float n1, float n2);
float realAverage(int r, int N);