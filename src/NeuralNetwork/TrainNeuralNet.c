#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#pragma GCC diagnostic pop

#include "NeuralNet.h"
#include "NeuralNetInit.h"
#include "MatUtils.h"
#include "LoadDataSet.h"

// Gradient descent, m is the number of training sample
// New weight  =  old weight-(stepSize/m)*(nabla of the weight)
// New bias    =  old bias-(stepSize/m)*(nabla of the bias)
void gradientDescent(struct NeuralNetwork* nnPtr){
    double stepSize = 0.1;//TODO chanche only for debug
    for(int iHeight = 0;iHeight<nnPtr->nbNBL[0];iHeight++){
        for(int iWidth = 0; iWidth<nnPtr->nbNBL[1];iWidth++){
            nnPtr->wh[iHeight*nnPtr->nbNBL[1]+iWidth]  -= stepSize*nnPtr->nablaWh[iHeight*nnPtr->nbNBL[1]+iWidth];
            nnPtr->bh[iWidth] -= stepSize*nnPtr->nablaBh[iWidth];
        }
    }
    for(int iHeight = 0;iHeight<nnPtr->nbNBL[1];iHeight++){
        for(int iWidth = 0; iWidth<nnPtr->nbNBL[2];iWidth++){
            nnPtr->wy[iHeight*nnPtr->nbNBL[2]+iWidth] -= stepSize*nnPtr->nablaWy[iHeight*nnPtr->nbNBL[2]+iWidth];
            nnPtr->by[iWidth] -= stepSize*nnPtr->nablaBy[iWidth];
        }
    }
}

void sigmoidPrime(double* h, int width, struct NeuralNetwork* nnPtr){
    for(int i = 0;i<width;i++)
        nnPtr->hAP[i] = 1;
    matSub(nnPtr->hAP, h, nnPtr->hAP, 1, width);
    hadamardProduct(nnPtr->hAP, h, 1, width);
}
// Calculate the output error: error =  y-y*
void gradErrorL(double* y, double* tO, double* res, int k){
    matSub(y, tO, res, 1, k);
}
// Propagate the output error to the hidden layer
double* gradErrorH(double* error, double* w, double* nBh, struct NeuralNetwork* nnPtr){
    matTranspose(w, nnPtr->wT, nnPtr->nbNBL[1], nnPtr->nbNBL[2]);
    matMult(error, nnPtr->wT, 1, nnPtr->nbNBL[2], nnPtr->nbNBL[1], nBh);
    sigmoidPrime(nnPtr->hA, nnPtr->nbNBL[1], nnPtr);
    hadamardProduct(nBh, nnPtr->hAP, 1, nnPtr->nbNBL[1]);
    return nBh;
}
// Back propagation: for each layer propagate the error of the predicted output
void backPropagation(struct NeuralNetwork* nnPtr, double* targetOutput){
    gradErrorL(nnPtr->yA, targetOutput, nnPtr->nablaBy, nnPtr->nbNBL[2]);
    double* error = nnPtr->nablaBy;
    double* errorH = gradErrorH(error, nnPtr->wy, nnPtr->nablaBh, nnPtr);
    //hA is transposed so we invert dim
    matMult(nnPtr->hA, error,nnPtr->nbNBL[1], 1, nnPtr->nbNBL[2], nnPtr->nablaWy);
    //input is transposed so we invert dim
    matMult(nnPtr->input, errorH, nnPtr->nbNBL[0], 1, nnPtr->nbNBL[1], nnPtr->nablaWh);
}

void creatMiniBatch(int size){
    size=size;
    //MINI BATCH
    //On mélange les images
    //ensuite on crée plein de data set j'usqua la fin de toutes les images
    //LOAD IMAGE
    //UDATE BACKPROP FOR EACH IMAGE IN THE MINI batch
    //AT THE END ONE SINGEL STEP GRAD
}

// Train the neural network
void trainNn(int iterationLimit, char* filename){
    //TODO if does not converge change the wb
    int* nbNBL = calloc(3, sizeof(int));
    nbNBL[0]  =  784; 
    nbNBL[1]  =  30; 
    nbNBL[2]  =  10;
    double* input = calloc(nbNBL[0], sizeof(double));
    struct NeuralNetwork* nnPtr =  initNn(nbNBL, input);
    int iterationNum = 0;
    while(iterationNum<iterationLimit){
        //feedForward(nnPtr);
        //backPropagation(nnPtr, targetOutput);
        //gradientDescent(nnPtr);
        /*if(iterationNum==iterationLimit-1)
        {
            printf("I1 = %d, I2 = %d, T = %d, 0 = %f\n\n",
            (int)nnPtr->input[0], (int)nnPtr->input[1], 
            (int)sampleList[iSample+2], nnPtr->yA[0]);
        }*/
        iterationNum +=  1;
    }
    saveNn(filename, nnPtr);
    freeNn(nnPtr);
    free(nnPtr);
}
