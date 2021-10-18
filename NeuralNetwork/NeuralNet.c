#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "NeuralNet.h"

//put this in utils----------------------------------------------------------
double randomDouble(){
    #ifndef RANDOMSEED
    #define RANDOMSEED srand(time(NULL));
    #endif
    double x = (double)rand()/(double)(RAND_MAX);
    return x;
}
// Multiply matA * matB
void matricesMult(double* matA, double* matB,int heightA,int widthA, int widthB, double* result) {
    for(int iHeightA=0;iHeightA<heightA;iHeightA++){
        for(int iWidthB=0;iWidthB<widthB;iWidthB++){
            double sum=0;
            for(int iWidthA=0;iWidthA<widthA;iWidthA++){
                sum = sum + matA[iHeightA*widthA+iWidthA] * matB[iWidthA*widthB+iWidthB];
            }
            result[iHeightA*widthB+iWidthB] = sum;
        }   
    } 
}
// Add matA (column) + matB (column)
void matricesCAdd(double* matA, double *matB, int heightA) {
    for(int iHeight=0;iHeight<heightA;iHeight++){
        matA[iHeight]+=matB[iHeight];
    }
} 
// Tranpose a mat
void matTranspose(double* mat, double* result, int height, int width){
    for(int iHeight = 0; iHeight<height*width; iHeight++) {
        int i = iHeight/height;
        int j = iHeight%height;
        result[iHeight] = mat[width*j + i];
    }
}
//------------------------------------------------------------------
// Initialize the weight with uniform distribution and the bias to 0
void initWB(struct NeuralNetwork* nnPtr) {
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[1]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[0]; iWidth++) {
            nnPtr->w1[iHeight * nnPtr->nbNBL[0] + iWidth] = randomDouble()/sqrt(nnPtr->nbNBL[0]);
            nnPtr->b1[iWidth] = 0;
        }
    }
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[2]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[1]; iWidth++) {
            nnPtr->w2[iHeight * nnPtr->nbNBL[1] + iWidth] = randomDouble()/sqrt(nnPtr->nbNBL[1]);
            nnPtr->b2[iWidth] = 0;
        }
    }
}

struct NeuralNetwork* initNn(int* nbNBL,double* input){
    struct NeuralNetwork* nnPtr = malloc(sizeof(struct NeuralNetwork));
    struct NeuralNetwork nn = *nnPtr;
    
    nn.input = input;
    nn.inputA = calloc(nbNBL[0],sizeof(double));

    nn.nbNBL = nbNBL;// Number of neurone by layer
    
    nn.w1 = calloc(nn.nbNBL[1]*nn.nbNBL[0],sizeof(double));// Do not forget to free height*width
    nn.w2 = calloc(nn.nbNBL[2]*nn.nbNBL[1],sizeof(double));
    
    nn.b1 = calloc(nn.nbNBL[1],sizeof(double));
    nn.b2 = calloc(nn.nbNBL[2],sizeof(double));
    
    nn.outputLayer = calloc(nn.nbNBL[2],sizeof(double));
    nn.hiddenLayer = calloc(nn.nbNBL[1],sizeof(double));
    
    nn.hiddenLayerA = calloc(nn.nbNBL[1],sizeof(double));
    nn.outputLayerA = calloc(nn.nbNBL[2],sizeof(double));
    initWB(nnPtr);
    return nnPtr;
}

// Activation function: Softmax
double activationF(int height, double x, double* mat){
    long double sum=0;
    for(int i=0;i<height;i++){
        sum+=exp(mat[i]);
    }
    return exp(x)/sum;
}
// Derivative of the activation function
double activationFDerivative(int height, double x, double* mat){
    return activationF(height,x,mat)*(1-activationF(height,x,mat));
}

double* activationFDerivativeL(int height, double* mat){
    double* result=calloc(height,sizeof(double));
    for(int iHeight=0;iHeight<height;iHeight++){
        result[iHeight] = activationFDerivative(height,mat[iHeight],mat);
    }
    return result;
}

// Feedforward
void feedForward(struct NeuralNetwork* nnPtr){
    for(int iWidth=0;iWidth<nnPtr->nbNBL[0];iWidth++){
        nnPtr->inputA[iWidth]=activationF(nnPtr->nbNBL[0],nnPtr->input[iWidth],nnPtr->input);
    }
    matricesMult(nnPtr->w1,nnPtr->input,nnPtr->nbNBL[1],nnPtr->nbNBL[0],1,nnPtr->hiddenLayer);
    matricesCAdd(nnPtr->hiddenLayer,nnPtr->b1,nnPtr->nbNBL[1]);
    for(int iWidth=0;iWidth<nnPtr->nbNBL[1];iWidth++){
        nnPtr->hiddenLayerA[iWidth]=activationF(nnPtr->nbNBL[1],nnPtr->hiddenLayer[iWidth],nnPtr->hiddenLayer);
    }
    matricesMult(nnPtr->w2,nnPtr->hiddenLayerA,nnPtr->nbNBL[2],nnPtr->nbNBL[1],1,nnPtr->outputLayer);
    matricesCAdd(nnPtr->outputLayer,nnPtr->b2,nnPtr->nbNBL[2]);//TODO verifier renvoi un scalaire
    for(int iWidth=0;iWidth<nnPtr->nbNBL[2];iWidth++){
        nnPtr->outputLayerA[iWidth]=activationF(nnPtr->nbNBL[2],nnPtr->outputLayer[iWidth],nnPtr->outputLayer);
    }
}

// Hadamard product
double* hadamardProduct(double* matA, double* matB,int width, int height){
    double* result  = calloc(width*height,sizeof(double));//DO NOT FORGET TO FREE
    for(int iHeight=0;iHeight<height;iHeight++){
        for(int iWidth=0; iWidth<width;iWidth++){
            result[iHeight*width+iHeight] = matA[iHeight*width+iWidth] * matB[iHeight*width+iWidth];
        }
    }
    return result;
}
// Gradient descent, m is the number of training sample
void gradientDescent(struct NeuralNetwork* nnPtr,double* nablaB2, double* nablaW2, double* nablaB1, double* nablaW1, double m){
    double stepSize=0.01;
    //TODO vérifier tout les index ex: nablaW1
    for(int iHeight=0;iHeight<nnPtr->nbNBL[1];iHeight++){
        for(int iWidth=0; iWidth<nnPtr->nbNBL[0];iWidth++){
            nnPtr->w1[iHeight*nnPtr->nbNBL[0]+iWidth]-=stepSize*((1/m)*nablaW1[iHeight]);
            nnPtr->b1[iWidth]-=stepSize*((1/m)*nablaB1[iWidth]);
        }
    }
    for(int iHeight=0;iHeight<nnPtr->nbNBL[2];iHeight++){
        for(int iWidth=0; iWidth<nnPtr->nbNBL[1];iWidth++){
            nnPtr->w2[iHeight*nnPtr->nbNBL[1]+iWidth]-=stepSize*((1/m)*nablaW2[iHeight]);
            nnPtr->b2[iWidth]-=stepSize*((1/m)*nablaB2[iWidth]);
        }
    }
}
// Derivative of the cost
double* costDerivative(double* targetOutput, double* output, int height){
    double* result = calloc(height,sizeof(double));//DO not forget to free
    for(int iHeight=0;iHeight<height;iHeight++){
        result[iHeight] = output[iHeight] - targetOutput[iHeight];
    }
    return result;
}
// Back propagation 
void backPropagation(struct NeuralNetwork* nnPtr, double* targetOutput, double* nablaB2, double* nablaW2, double* nablaB1, double* nablaW1){
    // Calculate the output error
    int outputHeight=nnPtr->nbNBL[2];
    double* adO = activationFDerivativeL(outputHeight,nnPtr->outputLayer);
    double* cd = costDerivative(targetOutput,nnPtr->outputLayerA,outputHeight);
    double* errorOutputLayer = hadamardProduct(cd,adO,1,outputHeight);
    free(adO);
    nablaB2 = errorOutputLayer;                      
    matricesMult(nnPtr->hiddenLayerA,errorOutputLayer,nnPtr->nbNBL[2],nnPtr->nbNBL[1],1,nablaW2);// be carefule transpose
    // Back propagate the error to the hidden layer
    int hiddenLHeight=nnPtr->nbNBL[1];
    double* adH = activationFDerivativeL(hiddenLHeight,nnPtr->hiddenLayer);
    double* r = calloc(nnPtr->nbNBL[2],sizeof(double));
    double* wT = calloc(nnPtr->nbNBL[1],sizeof(double));
    matTranspose(nnPtr->w2,wT,nnPtr->nbNBL[2],nnPtr->nbNBL[1]);
    matricesMult(wT,errorOutputLayer,nnPtr->nbNBL[1],nnPtr->nbNBL[2],1,r);
    double* errorHiddenLayer = hadamardProduct(r,adH,1,hiddenLHeight);
    free(cd);
    free(adH);
    free(wT);
    free(r);
    nablaB1 = errorHiddenLayer;
    matricesMult(nnPtr->inputA,errorHiddenLayer,nnPtr->nbNBL[0],1,1,nablaW1);
}

// Train the neural network
void trainNn(int iterationLimit){
    // This is only for testing uses 
    //----------------------------------------
    double* targetOutput=calloc(1,sizeof(double));
    targetOutput[0]=0;
    int* nbNBL=calloc(3, sizeof(3));
    nbNBL[0] = 2; nbNBL[1] = 2; nbNBL[2] = 1;
    double* input=calloc(2, sizeof(int));
    input[0] = 0; input[1] = 0;
    //----------------------------------------
    double* nablaB2=calloc(nbNBL[2],sizeof(double));
    double* nablaW2=calloc(nbNBL[2]*nbNBL[1],sizeof(double));
    double* nablaB1=calloc(nbNBL[1],sizeof(double));
    double* nablaW1=calloc(nbNBL[1]*nbNBL[0],sizeof(double));
    
    struct NeuralNetwork* nnPtr= initNn(nbNBL,input);
    initWB(nnPtr);
    int iterationNum=0;
    int nbSample=1;
    while(iterationNum<iterationLimit){
        //nablaB2=nablaW2=nablaB1=nablaW1=[0,...];
        for(int iSample=0;iSample<nbSample;iSample++){
            feedForward(nnPtr);
	        backPropagation(nnPtr,targetOutput,nablaB2,nablaW2,nablaB1,nablaW1);//ne pas écraser les nabla à chaque fois
	        gradientDescent(nnPtr,nablaB2,nablaW2,nablaB1,nablaW1,nbSample);
            //average cost?
        }
        iterationNum+=1;
    }
}

//TODO functions load/save/printNn
