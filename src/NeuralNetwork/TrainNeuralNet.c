#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#pragma GCC diagnostic pop

#include "NeuralNet.h"
#include "NeuralNetInit.h"
#include "MatUtils.h"

// Gradient descent, m is the number of training sample
// New weight  =  old weight-(stepSize/m)*(nabla of the weight)
// New bias    =  old bias-(stepSize/m)*(nabla of the bias)
void gradientDescent(struct NeuralNetwork* nnPtr){
    double stepSize = 0.1;//0.25
    for(int iHeight = 0;iHeight<nnPtr->nbNBL[0];iHeight++){
        for(int iWidth = 0; iWidth<nnPtr->nbNBL[1];iWidth++){
            nnPtr->wh[iHeight*nnPtr->nbNBL[1]+iWidth]  -= stepSize*nnPtr->nablaWh[iHeight*nnPtr->nbNBL[1]+iWidth];
            nnPtr->bh[iWidth] -= stepSize*nnPtr->nablaBh[iWidth];
        }
    }
    for(int iHeight = 0;iHeight<nnPtr->nbNBL[1];iHeight++){
        for(int iWidth = 0; iWidth<nnPtr->nbNBL[2];iWidth++){
            nnPtr->wy[iHeight*nnPtr->nbNBL[2]+iWidth] += stepSize*nnPtr->nablaWy[iHeight*nnPtr->nbNBL[2]+iWidth];
            nnPtr->by[iWidth] -= stepSize*nnPtr->nablaBy[iWidth];
        }
    }
}
double* sigmoidPrime(double* h, int width){
    double* res = malloc(sizeof(double)*width);
    for(int i = 0;i<width;i++){
        res[i] = 1;
    }
    matSub(res, h, res, 1, width);
    return hadamardProduct(res, h, 1, width);
}
// Calculate the output error: error =  y-y*
void gradErrorL(double* y, double* tO, double* res, int k){
    matSub(res, y, tO, 1, k);
}
// Propagate the output error to the hidden layer
double* gradErrorH(double* matGradErrorL, double* w, struct NeuralNetwork* nnPtr){
    double* r = malloc(sizeof(double)*nnPtr->nbNBL[1]);
    double* wT=malloc(sizeof(double)*nnPtr->nbNBL[2]*nnPtr->nbNBL[1]);
    matTranspose(w, wT, nnPtr->nbNBL[1], nnPtr->nbNBL[2]);
    matMult(matGradErrorL, wT, 1, nnPtr->nbNBL[2], nnPtr->nbNBL[1], r);
    return hadamardProduct(r, sigmoidPrime(nnPtr->hA, nnPtr->nbNBL[1]), 1, nnPtr->nbNBL[1]);
}
// Back propagation: for each layer propagate the error of the predicted output
void backPropagation(struct NeuralNetwork* nnPtr, double* targetOutput){
    double* matGradErrorL = malloc(sizeof(double)*nnPtr->nbNBL[2]) ;
    gradErrorL(nnPtr->yA, targetOutput, matGradErrorL, nnPtr->nbNBL[2]);
    double* matGradErrorHL = gradErrorH(matGradErrorL, nnPtr->wy, nnPtr);
    double* r = malloc(sizeof(double)*nnPtr->nbNBL[1]*nnPtr->nbNBL[2]);
    //hA is transposed so we invert dim
    matMult(nnPtr->hA, matGradErrorL,nnPtr->nbNBL[1], 1, nnPtr->nbNBL[2], r);
    free(nnPtr->nablaBy);
    free(nnPtr->nablaWy);
    nnPtr->nablaWy = r;
    nnPtr->nablaBy = matGradErrorL;
    double* uT = nnPtr->input;//TODO activate input?
    double* r1 = malloc(sizeof(double)*nnPtr->nbNBL[0]*nnPtr->nbNBL[1]);
    matMult(uT, matGradErrorHL, nnPtr->nbNBL[0], 1, nnPtr->nbNBL[1], r1);
    free(nnPtr->nablaBh);
    free(nnPtr->nablaWh);
    nnPtr->nablaWh = r1;
    nnPtr->nablaBh = matGradErrorHL;
}

// Creat sample: two inputs 0 or 1 and the target output
double* creatSample(int nbSample){
    double* sampleList = calloc(nbSample*3, sizeof(double));
    sampleList[0]  =  0;
    sampleList[1]  =  0;
    sampleList[2]  =  0;
    sampleList[3]  =  1;
    sampleList[4]  =  1;
    sampleList[5]  =  0;
    sampleList[6]  =  1;
    sampleList[7]  =  0;
    sampleList[8]  =  1;
    sampleList[9]  =  0;
    sampleList[10] =  1;
    sampleList[11] =  1;
    return sampleList;
}
// Train the neural network
void trainNn(int iterationLimit, char* filename){
    int nbSample = 4;
    double* targetOutput = calloc(1, sizeof(double));
    double* sampleList = creatSample(nbSample);
    int* nbNBL = calloc(3, sizeof(3));
    nbNBL[0]  =  2; nbNBL[1]  =  2; nbNBL[2]  =  1;
    double* input = calloc(2, sizeof(double));
    struct NeuralNetwork* nnPtr =  initNn(nbNBL, input);
    int iterationNum = 0;

    while(iterationNum<iterationLimit){
        for(int iSample = 0; iSample<nbSample*3; iSample +=  3){
            nnPtr->input[0] = sampleList[iSample];
            nnPtr->input[1] = sampleList[iSample+1];
            targetOutput[0] = sampleList[iSample+2];
            feedForward(nnPtr);
	        backPropagation(nnPtr, targetOutput);
            gradientDescent(nnPtr);
            if(iterationNum == iterationLimit-1){
                printf("I1 = %f, I2 = %f, T = %f, O = %f\n", 
                sampleList[iSample], sampleList[iSample+1], 
                sampleList[iSample+2], nnPtr->yA[0]);
            }
        }
        iterationNum +=  1;
    }
    saveNn(filename, nnPtr);
    free(nbNBL);
    free(input);
    free(targetOutput);
    freeNn(nnPtr);
    free(nnPtr);
    free(sampleList);
}
