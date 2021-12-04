#ifndef MATUTILS
#define MATUTILS
void printMat(double* mat, int height, int width);
void matMult(double* matA, double* matB,int heightA,int widthA, int widthB, double* result);
void matSub(double* matA, double* matB, double* matC, int height,int width);
void matAdd(double* matA, double* matB, int height, int width);
void matTranspose(double* mat, double* result, int height, int width);
void hadamardProduct(double* matA, double* matB, int height, int width);
#endif