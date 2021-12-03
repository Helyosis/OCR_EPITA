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
    double stepSize = 0.1;//0.01 test
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
double* sigmoidPrime(double* h, int width){
    double* res = calloc(width,sizeof(double));
    for(int i = 0;i<width;i++){
        res[i] = 1;
    }
    matSub(res, h, res, 1, width);
    return hadamardProduct(res, h, 1, width);
}
// Calculate the output error: error =  y-y*
void gradErrorL(double* y, double* tO, double* res, int k){
    matSub(y, tO, res, 1, k);
}
// Propagate the output error to the hidden layer
double* gradErrorH(double* error, double* w, struct NeuralNetwork* nnPtr){
    double* r = calloc(nnPtr->nbNBL[1],sizeof(double));
    double* wT= calloc(nnPtr->nbNBL[2]*nnPtr->nbNBL[1],sizeof(double));
    matTranspose(w, wT, nnPtr->nbNBL[1], nnPtr->nbNBL[2]);
    matMult(error, wT, 1, nnPtr->nbNBL[2], nnPtr->nbNBL[1], r);
    free(wT);
    return hadamardProduct(r, sigmoidPrime(nnPtr->hA, nnPtr->nbNBL[1]), 1, nnPtr->nbNBL[1]);
}
// Back propagation: for each layer propagate the error of the predicted output
void backPropagation(struct NeuralNetwork* nnPtr, double* targetOutput){
    double* error = calloc(nnPtr->nbNBL[2],sizeof(double)) ;
    gradErrorL(nnPtr->yA, targetOutput, error, nnPtr->nbNBL[2]);
    double* errorH = gradErrorH(error, nnPtr->wy, nnPtr);
    double* r = calloc(nnPtr->nbNBL[1]*nnPtr->nbNBL[2],sizeof(double));
    //hA is transposed so we invert dim
    matMult(nnPtr->hA, error,nnPtr->nbNBL[1], 1, nnPtr->nbNBL[2], r);
    free(nnPtr->nablaBy);
    free(nnPtr->nablaWy);
    nnPtr->nablaWy = r;
    nnPtr->nablaBy = error;
    double* uT = nnPtr->input;//TODO activate input?
    double* r1 = calloc(nnPtr->nbNBL[0]*nnPtr->nbNBL[1],sizeof(double));
    matMult(uT, errorH, nnPtr->nbNBL[0], 1, nnPtr->nbNBL[1], r1);
    free(nnPtr->nablaBh);
    free(nnPtr->nablaWh);
    nnPtr->nablaWh = r1;
    nnPtr->nablaBh = errorH;
}

// Creat sample: two inputs 0 or 1 and the target output
double* creatSample(int nbSample){
    double* sampleList = calloc(nbSample*3, sizeof(double));
    int k=0;
    for (int j=0;j<2;j++){
        for (int i=0;i<2;i++){
            sampleList[k] = i;
            sampleList[k+1] = j;
            sampleList[k+2] = i^j;
            k+=3;
        }
    }
    return sampleList;
}
// Train the neural network
void trainNn(int iterationLimit, char* filename){
    int nbSample = 4;
    double* sampleList = creatSample(nbSample);
    int* nbNBL = calloc(3, sizeof(3));
    nbNBL[0]  =  2; nbNBL[1]  =  2; nbNBL[2]  =  1;
    double* input = calloc(2, sizeof(double));
    double* targetOutput = calloc(nbNBL[2], sizeof(double));
    struct NeuralNetwork* nnPtr =  initNn(nbNBL, input);
    int iterationNum = 0;
    while(iterationNum<iterationLimit){
        for(int iSample = 0; iSample<nbSample*3; iSample +=  3){
            nnPtr->input[0] =  sampleList[iSample];
            nnPtr->input[1] =  sampleList[iSample+1];
            if((int)sampleList[iSample+2]==1)
            {
                targetOutput[0] = 1;
                //targetOutput[1] = 1;
            }else{
                targetOutput[0] = 0;
                //targetOutput[1] = 0;
            }
            feedForward(nnPtr);
	        backPropagation(nnPtr, targetOutput);
            gradientDescent(nnPtr);
            printf("I1 = %d, I2 = %d, T = %d, 0 = %f\n\n",
            (int)nnPtr->input[0], (int)nnPtr->input[1], 
            (int)sampleList[iSample+2], nnPtr->yA[0]);
        }
        saveNn(filename, nnPtr);
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
