#ifndef MATUTILS
#define MATUTILS
void printMat(double* mat, int height, int width);
void matricesMult(double* matA, double* matB,int heightA,int widthA, int widthB, double* result);
void matricesCAdd(double* matA, double *matB, int heightA);
void matricesAdd(double* matA, double* matB, int height,int width);
void matTranspose(double* mat, double* result, int height, int width);
double* hadamardProduct(double* matA, double* matB,int width, int height);
#endif