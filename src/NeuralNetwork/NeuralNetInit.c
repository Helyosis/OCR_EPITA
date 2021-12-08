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

double randomDouble(double fMax, double fMin){
    //double x = (double)rand()/(double)(RAND_MAX);
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

// Initialize the weight with uniform distribution: 
//(random number)/sqrt(nb neurone of the layer) and the bias to 0
void initWB(struct NeuralNetwork* nnPtr) {
    srand(time(NULL));
    //double fMin=-sqrt(6/(nnPtr->nbNBL[0]+nnPtr->nbNBL[1]));
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[0]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[1]; iWidth++) {
            //Xavier/Glorot Uniform Initialization (better for sigmoid activation)
            nnPtr->wh[iHeight * nnPtr->nbNBL[1] + iWidth] = randomDouble(-1,1);
//          if(iHeight == 0)
//              nnPtr->bh[iWidth] = -1+2*((double)rand())/RAND_MAX;
        }
    }
    //fMin=-1/sqrt(nnPtr->nbNBL[1]);
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[1]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[2]; iWidth++) {
            //Uniform Initialization
            nnPtr->wy[iHeight * nnPtr->nbNBL[2] + iWidth] = randomDouble(-1,1);
            //-1/sqrt(nnPtr->nbNBL[1]) + 2 * randomDouble()/sqrt(nnPtr->nbNBL[1]);
//          if(iHeight == 0)
//              nnPtr->by[iWidth] =  -1+2*((double)rand())/RAND_MAX;
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
    nnPtr->nablaWy = calloc(nnPtr->nbNBL[1]*nnPtr->nbNBL[2],sizeof(double));
    nnPtr->nablaBh = calloc(nnPtr->nbNBL[1],sizeof(double));
    nnPtr->nablaWh = calloc(nnPtr->nbNBL[0]*nnPtr->nbNBL[1],sizeof(double));

    nnPtr->nablaWyI = calloc(nnPtr->nbNBL[1]*nnPtr->nbNBL[2],sizeof(double));
    nnPtr->nablaWhI = calloc(nnPtr->nbNBL[0]*nnPtr->nbNBL[1],sizeof(double));

    nnPtr->tOutput = calloc(nnPtr->nbNBL[2], sizeof(double));
    nnPtr->error = calloc(nnPtr->nbNBL[2],sizeof(double));
    nnPtr->errorH = calloc(nnPtr->nbNBL[1],sizeof(double));
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
    free(nnPtr->nablaWyI);
    free(nnPtr->nablaWhI);
    free(nnPtr->tOutput);
    free(nnPtr->error);
    free(nnPtr->errorH);
}
