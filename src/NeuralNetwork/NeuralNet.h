#ifndef NEURALNET
#define NEURALNET
#include "NeuralNetInit.h"
void feedForward(struct NeuralNetwork* nnPtr);
double activationF(double x);
void saveNn(char* fileName, struct NeuralNetwork* nnPtr);
//void loadNn(char* fileName,struct NeuralNetwork* nnPtr);
#endif
