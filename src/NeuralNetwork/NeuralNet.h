#ifndef NEURALNET
#define NEURALNET
#include "NeuralNetInit.h"
void feedForward(struct NeuralNetwork* nnPtr);
void sigmoid(double* mA,double* m, int width);
void saveNn(char* fileName, struct NeuralNetwork* nnPtr);
struct NeuralNetwork* loadNn(char* fileName);
#endif
