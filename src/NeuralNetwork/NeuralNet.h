#ifndef NEURALNET
#define NEURALNET
#include "NeuralNetInit.h"
#define PATH_WEIGHT = "./weights/"
void feedForward(struct NeuralNetwork* nnPtr);
void sigmoid(double* mA,double* m, int width);
void saveNn(char* fileName, struct NeuralNetwork* nnPtr);
//void loadNn(char* fileName,struct NeuralNetwork* nnPtr);
#endif
