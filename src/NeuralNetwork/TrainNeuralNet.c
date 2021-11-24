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

//TODO
// Gradient descent, m is the number of training sample
// New weight  =  old weight-(stepSize/m)*(nabla of the weight)
// New bias    =  old bias-(stepSize/m)*(nabla of the bias)
void gradientDescent(struct NeuralNetwork* nnPtr){
    double stepSize = 0.1;//0.25
    for(int iHeight = 0;iHeight<nnPtr->nbNBL[1];iHeight++){
        for(int iWidth = 0; iWidth<nnPtr->nbNBL[0];iWidth++){
            nnPtr->w1[iHeight*nnPtr->nbNBL[0]+iWidth] +=  stepSize*nnPtr->nablaW1[iHeight*nnPtr->nbNBL[0]+iWidth];
            nnPtr->b1[iHeight] +=  -stepSize*nnPtr->nablaB1[iHeight];
        }
    }
    for(int iHeight = 0;iHeight<nnPtr->nbNBL[2];iHeight++){
        for(int iWidth = 0; iWidth<nnPtr->nbNBL[1];iWidth++){
            nnPtr->w2[iHeight*nnPtr->nbNBL[1]+iWidth] +=  stepSize*nnPtr->nablaW2[iHeight*nnPtr->nbNBL[1]+iWidth];
            nnPtr->b2[iHeight] +=  -stepSize*nnPtr->nablaB2[iHeight];
        }
    }
}
double* sigmoidPrime(double* h, int height){
    double* result=malloc(sizeof(double)*height);
    for(int i=0;i<height;i++){
        result[i]=1;
    }
    matricesCSub(result,h,height);
    return hadammardProduct(result,h,1,height);
}
// Calculate the output error
void gradErrorL(double* y, double* error, NeuralNetwork* nnPtr){
    matricesCSub(error,y,nnPtr->nbNBL[2]);
}
// Propagate the output error to the hidden layer
void gradErrorH(double* matGradErrorL, double* w, struct NeuralNetwork* nnPtr){
    double* r = malloc(sizeof(double)*);
    double* wT=malloc(sizeof(double)*);
    matTranspose(w,wT,,);
    matMult(matGradErrorL,wT,r,,);
    return hadamardProduct(r,sigmoidPrime(nnPtr->hiddenLayerA,nnPtr->nbNBL[1]),,);
}
// Back propagation: for each layer propagate the error of the predicted output
void backPropagation(struct NeuralNetwork* nnPtr, double* targetOutput){
    double* matGradErrorL  =  gradErrorL(nnPtr->outputLayerA[0],errorOutputLayer);
    double errorGradHiddenLayer = calculateGardErrorHiddenLayer(,);
    double* H = nnPtr->hiddenLayerA;
    double* r = malloc(sizeof(double)*nnPtr->nbNBL[2]*nnPtr->nbNBL[1]);
    nnPtr->nablaW2 = matMult(matGradErrorL,H,nnPtr->nbNBL[2],1,nnPtr->nbNBL[1],r);//TODO
    nnPtr->nablaB2 = matGradErrorL;
}

// Creat sample: two inputs 0 or 1 and the target output
double* creatSample(int nbSample){
    double* sampleList = calloc(nbSample*3,sizeof(double));
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
    sampleList[10]  =  1;
    sampleList[11]  =  1;
    return sampleList;
}
// Train the neural network
void trainNn(int iterationLimit, char* filename){
    int nbSample = 4;
    double* targetOutput = calloc(1,sizeof(double));
    double* sampleList = creatSample(nbSample);
    int* nbNBL = calloc(3, sizeof(3));
    nbNBL[0]  =  2; nbNBL[1]  =  2; nbNBL[2]  =  1;
    double* input = calloc(2, sizeof(double));
    struct NeuralNetwork* nnPtr =  initNn(nbNBL,input);
    int iterationNum = 0;

    while(iterationNum<iterationLimit){
        for(int iSample = 0;iSample<nbSample*3;iSample += 3){
            nnPtr->input[0] = sampleList[iSample];
            nnPtr->input[1] = sampleList[iSample+1];
            targetOutput[0] = sampleList[iSample+2];
            feedForward(nnPtr);
	        backPropagation(nnPtr,targetOutput);
            gradientDescent(nnPtr);
            if(iterationNum==iterationLimit-1){
            printf("I1 = %f, I2 = %f, T = %f, O = %f\n",
            sampleList[iSample],sampleList[iSample+1],
            sampleList[iSample+2],nnPtr->outputLayerA[0]);
            }
        }
        iterationNum += 1;
    }
    saveNn(filename,nnPtr);
    free(nbNBL);
    free(input);
    free(targetOutput);
    freeNn(nnPtr);
    free(nnPtr);
    free(sampleList);
}
