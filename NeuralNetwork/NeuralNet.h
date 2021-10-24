#ifndef NEURALNET
#define NEURALNET

void feedForward(struct NeuralNetwork* nnPtr);
double activationF(double x);
/*struct NeuralNetwork* initNn();
void gradientDescent(struct NeuralNetwork* nnPtr,double* nablaB2, double* nablaW2, double* nablaB1, double* nablaW1, int m);
void backPropagation(struct NeuralNetwork* nnPtr, double* targetOutput, double* nablaB2, double* nablaW2, double* nablaB1, double* nablaW1);
double* costDerivative(double* targetOutput, double* output, int height);*/
#endif
