#ifndef NEURALNETINIT
#define NEURALNETINIT
struct NeuralNetwork
{
  double* input;
  double* inputA;
  int* nbNBL;
  double* w1;
  double* w2;
  double* b1;
  double* b2;
  double* hiddenLayer;
  double* hiddenLayerA;
  double* outputLayer;
  double* outputLayerA;
  double* nablaB2;
  double* nablaW2;
  double* nablaB1;
  double* nablaW1;
};
struct NeuralNetwork* initNn(int* nbNBL,double* input);
void freeNn(struct NeuralNetwork* nnPtr);
#endif