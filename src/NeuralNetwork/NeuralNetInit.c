#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#pragma GCC diagnostic pop

#include "NeuralNetInit.h"

double randomDouble(){
    #ifndef RANDOMSEED
    #define RANDOMSEED srand(time(NULL));
    #endif
    double x = (double)rand()/(double)(RAND_MAX);
    return x;
}

// Initialize the weight with uniform distribution: 
//(random number)/sqrt(nb neurone of the layer) and the bias to 0
void initWB(struct NeuralNetwork* nnPtr) {
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[1]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[0]; iWidth++) {
            nnPtr->w1[iHeight * nnPtr->nbNBL[0] + iWidth] = randomDouble()/sqrt(nnPtr->nbNBL[1]);
            nnPtr->b1[iHeight] = randomDouble()/sqrt(nnPtr->nbNBL[1]);
        }
    }
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[2]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[1]; iWidth++) {
            nnPtr->w2[iHeight * nnPtr->nbNBL[1] + iWidth] = randomDouble()/sqrt(nnPtr->nbNBL[1]);
            nnPtr->b2[iHeight] =  randomDouble()/sqrt(nnPtr->nbNBL[1]);
        }
    }
}

// Initialize the neural network
struct NeuralNetwork* initNn(int* nbNBL,double* input){
    struct NeuralNetwork* nnPtr = malloc(sizeof(struct NeuralNetwork));
    
    nnPtr->input = input;
    
    nnPtr->nbNBL = nbNBL;
    nnPtr->w1 = calloc(nnPtr->nbNBL[1]*nnPtr->nbNBL[0],sizeof(double));
    nnPtr->w2 = calloc(nnPtr->nbNBL[2]*nnPtr->nbNBL[1],sizeof(double));

    nnPtr->b1 = calloc(nnPtr->nbNBL[1],sizeof(double));
    nnPtr->b2 = calloc(nnPtr->nbNBL[2],sizeof(double));

    nnPtr->outputLayer = calloc(nnPtr->nbNBL[2],sizeof(double));
    nnPtr->hiddenLayer = calloc(nnPtr->nbNBL[1],sizeof(double));

    nnPtr->hiddenLayerA = calloc(nnPtr->nbNBL[1],sizeof(double));
    nnPtr->outputLayerA = calloc(nnPtr->nbNBL[2],sizeof(double));

    nnPtr->nablaB2=calloc(nbNBL[2],sizeof(double));
    nnPtr->nablaW2=calloc(nbNBL[2]*nbNBL[1],sizeof(double));
    nnPtr->nablaB1=calloc(nbNBL[1],sizeof(double));
    nnPtr->nablaW1=calloc(nbNBL[1]*nbNBL[0],sizeof(double));

    initWB(nnPtr);
    return nnPtr;
}

// Free the neural network
void freeNn(struct NeuralNetwork* nnPtr){
    free(nnPtr->w1);
    free(nnPtr->w2);
    free(nnPtr->b1);
    free(nnPtr->b2);
    free(nnPtr->outputLayer);
    free(nnPtr->hiddenLayer);
    free(nnPtr->hiddenLayerA);
    free(nnPtr->outputLayerA);
    free(nnPtr->nablaB2);
    free(nnPtr->nablaW2);
    free(nnPtr->nablaB1);
    free(nnPtr->nablaW1);
}
