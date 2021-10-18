#ifndef NEURALNET
#define NEURALNET
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
};
/*struct NeuralNetwork* initNn();
void initWB(struct NeuralNetwork* nnPtr);
void feedForward(struct NeuralNetwork* nnPtr);
void matricesMult(double* matA, double* matB,int heightA,int widthA, int widthB, double* result);
void matricesCAdd(double* matA, double *matB, int heightA);
void matTranspose(double* mat, double* result, int height, int width);
double activationF(int height, double x, double* mat);
double activationFDerivative(int height, double x, double* mat);
double* activationFDerivativeL(int height, double* mat);
double* hadamardProduct(double* matA, double* matB,int width, int height);
void gradientDescent(struct NeuralNetwork* nnPtr,double* nablaB2, double* nablaW2, double* nablaB1, double* nablaW1, int m);
void backPropagation(struct NeuralNetwork* nnPtr, double* targetOutput, double* nablaB2, double* nablaW2, double* nablaB1, double* nablaW1);
double* costDerivative(double* targetOutput, double* output, int height);*/
#endif
