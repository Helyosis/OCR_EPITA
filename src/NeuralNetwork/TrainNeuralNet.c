#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <time.h>
#pragma GCC diagnostic pop

#include "NeuralNet.h"
#include "NeuralNetInit.h"
#include "MatUtils.h"
#include "LoadDataSet.h"
#include "../Utils.h"
#include "../Verbose.h"

typedef struct NeuralNetwork* NN;
// Gradient descent, m is the number of training sample
// New weight  =  old weight-(stepSize/m)*(nabla of the weight)
// New bias    =  old bias-(stepSize/m)*(nabla of the bias)
void gradientDescent(NN nnPtr, int s, double learningRate){
    double stepSize = learningRate;
    for(int iHeight = 0;iHeight<nnPtr->nbNBL[0];iHeight++){
        for(int iWidth = 0; iWidth<nnPtr->nbNBL[1];iWidth++){
            nnPtr->wh[iHeight*nnPtr->nbNBL[1]+iWidth]  -= stepSize*(1/s)*nnPtr->nablaWh[iHeight*nnPtr->nbNBL[1]+iWidth];
            if(iHeight==0)
                nnPtr->bh[iWidth] -= stepSize*(1/s)*nnPtr->nablaBh[iWidth];
        }
    }
    for(int iHeight = 0;iHeight<nnPtr->nbNBL[1];iHeight++){
        for(int iWidth = 0; iWidth<nnPtr->nbNBL[2];iWidth++){
            nnPtr->wy[iHeight*nnPtr->nbNBL[2]+iWidth] -= stepSize*(1/s)*nnPtr->nablaWy[iHeight*nnPtr->nbNBL[2]+iWidth];
            if(iHeight==0)
                nnPtr->by[iWidth] -= stepSize*(1/s)*nnPtr->nablaBy[iWidth];
        }
    }
}

void sigmoidPrime(double* h, int width, NN nnPtr){
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
double* gradErrorH(double* error, double* w, double* nBh, NN nnPtr){
    matTranspose(w, nnPtr->wT, nnPtr->nbNBL[1], nnPtr->nbNBL[2]);
    matMult(error, nnPtr->wT, 1, nnPtr->nbNBL[2], nnPtr->nbNBL[1], nBh);
    sigmoidPrime(nnPtr->hA, nnPtr->nbNBL[1], nnPtr);
    hadamardProduct(nBh, nnPtr->hAP, 1, nnPtr->nbNBL[1]);
    return nBh;
}
// Back propagation: for each layer propagate the error of the predicted output
void backPropagation(NN nnPtr){
    gradErrorL(nnPtr->yA, nnPtr->tOutput, nnPtr->error, nnPtr->nbNBL[2]);
    matAdd(nnPtr->nablaBy,nnPtr->error,1,nnPtr->nbNBL[2]);
    double* errorH = gradErrorH(nnPtr->error, nnPtr->wy, nnPtr->errorH, nnPtr);
    matAdd(nnPtr->nablaBh,nnPtr->errorH,1,nnPtr->nbNBL[1]);
    //hA is transposed so we invert dim
    matMult(nnPtr->hA, nnPtr->error, nnPtr->nbNBL[1], 1, nnPtr->nbNBL[2], nnPtr->nablaWyI);
    matAdd(nnPtr->nablaWy, nnPtr->nablaWyI, nnPtr->nbNBL[1], nnPtr->nbNBL[2]);
    //input is transposed so we invert dim
    matMult(nnPtr->input, errorH, nnPtr->nbNBL[0], 1, nnPtr->nbNBL[1], nnPtr->nablaWhI);
    matAdd(nnPtr->nablaWh, nnPtr->nablaWhI, nnPtr->nbNBL[0], nnPtr->nbNBL[1]);
}

void swap(struct tImage* v1, struct tImage* v2){
    struct tImage tmp=*v1;
    *v1=*v2;
    *v2=tmp;
}

void shuffle(struct tImage** v, int size){
    for(int i=0;i<size;i++)
        swap(v[rand()%size],v[rand()%size]);
}

void updateMiniBatch(NN nnPtr, struct tImage** v, int l, int u, int s, double learningRate){
    for(; l<u ;l++){
        nnPtr->input=v[l]->pixVect;
        feedForward(nnPtr);
        nnPtr->tOutput[v[l]->label-1]=1;
        backPropagation(nnPtr);
        nnPtr->tOutput[v[l]->label-1]=0;
    }
    gradientDescent(nnPtr, s, learningRate);
}
//Set all the nabla/error/errorH to 0
void resetNabla(NN nnPtr){
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[0]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[1]; iWidth++) {
            nnPtr->nablaWh[iHeight * nnPtr->nbNBL[1] + iWidth] = 0;
            if( iHeight == 0)
                nnPtr->error[iWidth] = 0;
        }
    }
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[1]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[2]; iWidth++) {
            nnPtr->nablaWy[iHeight * nnPtr->nbNBL[2] + iWidth] = 0;
            if( iHeight == 0)
                nnPtr->errorH[iWidth] = 0;
        }
    }
}
// Train the neural network
void trainNn(t_options options){
    int iterationLimit = options.nbIterations;
    char* filename = options.outputFile;
    int* nbNBL = calloc(3, sizeof(int));
    nbNBL[0]  =  784; 
    nbNBL[1]  =  30; 
    nbNBL[2]  =  9;
    double* input = NULL;
    info_s("Init Neural Net weights and bias");
    NN nnPtr =  initNn(nbNBL, input);
    int size= options.nbImages;
    info_s("Load the Images");
    struct tImage** vect=imageVect(size);
    int sizeS = options.minibatch_size;
    for(int i=0;i<iterationLimit;i++){
        info_s("Iteration %d", i);
        //shuffle(vect, size);
        for(int s = 0; s<=size; s+=sizeS){
            int u=s+sizeS;
            if(u>size)
                u=size;
            updateMiniBatch(nnPtr, vect, s, u, sizeS, options.learningRate);
            resetNabla(nnPtr);
        }
        for(int j = 0; j < size ; j+=1){
            nnPtr->input = vect[j]->pixVect;
            feedForward(nnPtr);
            if (iterationLimit - 1 == j) {
                printMat(nnPtr->nablaBy, 1, nnPtr->nbNBL[2]);
            }
            else
                info_s("T=%d, O=%f",
                    vect[j]->label,nnPtr->yA[vect[j]->label-1]);
        }
    }
    freeImVect(vect, (size_t) size);
    saveNn(filename, nnPtr);
    freeNn(nnPtr);
    free(nnPtr);
}
