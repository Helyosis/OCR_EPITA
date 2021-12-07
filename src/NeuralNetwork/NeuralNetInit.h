#ifndef NEURALNETINIT
#define NEURALNETINIT
struct NeuralNetwork
{
  double* input;
  //double* inputA;
  int* nbNBL;
  double* wh;
  double* wT;
  double* wy;
  double* bh;
  double* by;
  double* y;
  double* h;
  double* hA;
  double* hAP;
  double* yA;
  double* nablaBy;
  double* nablaWy;
  double* nablaBh;
  double* nablaWh;
  double* nablaWyI;
  double* nablaWhI;
  double* tOutput;
  double* error;
  double* errorH;
};
struct NeuralNetwork* initNn(int* nbNBL,double* input);
void freeNn(struct NeuralNetwork* nnPtr);
void initWB(struct NeuralNetwork* nnPtr);
#endif