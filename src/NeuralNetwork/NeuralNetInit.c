#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#pragma GCC diagnostic pop

#include "NeuralNetInit.h"

double randomDouble(){
    #ifndef RANDOMSEED
    #define RANDOMSEED 
    srand(time(NULL));
    #endif
    //(rand() % (upper - lower + 1)) + lower
    double x = (double)rand()/(double)(RAND_MAX);
    return x;
}

// Initialize the weight with uniform distribution: 
//(random number)/sqrt(nb neurone of the layer) and the bias to 0
void initWB(struct NeuralNetwork* nnPtr) {
    srand(time(NULL));//TODO
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[0]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[1]; iWidth++) {
            nnPtr->wh[iHeight * nnPtr->nbNBL[1] + iWidth] =-(1/sqrt(nnPtr->nbNBL[0]))+2*randomDouble()/sqrt(nnPtr->nbNBL[0]);// randomDouble()/sqrt(nnPtr->nbNBL[1]);
//            if(iHeight == 0)
//                nnPtr->bh[iWidth] = -1+2*((double)rand())/RAND_MAX;
        }
    }
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[1]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[2]; iWidth++) {
            nnPtr->wy[iHeight * nnPtr->nbNBL[2] + iWidth] = -1/sqrt(nnPtr->nbNBL[1]) + 2 * randomDouble()/sqrt(nnPtr->nbNBL[1]);
//            if(iHeight == 0)
//                nnPtr->by[iWidth] =  -1+2*((double)rand())/RAND_MAX;
        }
    }
}

// Initialize the neural network
struct NeuralNetwork* initNn(int* nbNBL,double* input){
    struct NeuralNetwork* nnPtr = malloc(sizeof(struct NeuralNetwork));
    
    nnPtr->input = input;
    
    nnPtr->nbNBL = nbNBL;
    nnPtr->wh = calloc(nnPtr->nbNBL[0]*nnPtr->nbNBL[1],sizeof(double));
    nnPtr->wT = calloc(nnPtr->nbNBL[2]*nnPtr->nbNBL[1],sizeof(double));
    nnPtr->wy = calloc(nnPtr->nbNBL[1]*nnPtr->nbNBL[2],sizeof(double));

    nnPtr->bh = calloc(nnPtr->nbNBL[1],sizeof(double));
    nnPtr->by = calloc(nnPtr->nbNBL[2],sizeof(double));

    nnPtr->y = calloc(nnPtr->nbNBL[2],sizeof(double));
    nnPtr->h = calloc(nnPtr->nbNBL[1],sizeof(double));

    nnPtr->hA = calloc(nnPtr->nbNBL[1],sizeof(double));
    nnPtr->hAP = calloc(nnPtr->nbNBL[1],sizeof(double));
    nnPtr->yA = calloc(nnPtr->nbNBL[2],sizeof(double));

    nnPtr->nablaBy = calloc(nnPtr->nbNBL[2],sizeof(double));
    nnPtr->nablaWy = calloc(nnPtr->nbNBL[1]*nbNBL[2],sizeof(double));
    nnPtr->nablaBh = calloc(nnPtr->nbNBL[1],sizeof(double));
    nnPtr->nablaWh = calloc(nnPtr->nbNBL[0]*nbNBL[1],sizeof(double));

    nnPtr->tOutput = calloc(nbNBL[2], sizeof(double));
    initWB(nnPtr);
    return nnPtr;
}

// Free the neural network
void freeNn(struct NeuralNetwork* nnPtr){
    free(nnPtr->nbNBL);
    free(nnPtr->wh);
    free(nnPtr->wT);
    free(nnPtr->wy);
    free(nnPtr->bh);
    free(nnPtr->by);
    free(nnPtr->y);
    free(nnPtr->h);
    free(nnPtr->hA);
    free(nnPtr->hAP);
    free(nnPtr->yA);
    free(nnPtr->nablaBy);
    free(nnPtr->nablaWy);
    free(nnPtr->nablaBh);
    free(nnPtr->nablaWh);
    free(nnPtr->tOutput);
}
