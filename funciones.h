#include <math.h>
#include <stdio.h>

int getIndexProccess(int nProcesses, int discWidth, float distance);
float originDistance(float u, float v);
float realAverage(int r, int N);
float sumatoria (float n1, float n2);
void escribirSalida(int nProcesses, char* b, float datos[nProcesses][5]);