#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "NeuralNetInit.h"
#include "MatUtils.h"

// Activation function: sigmoid
double activationF(double x){
    return 1/(1+exp(-x));
}

// Feedforward
void feedForward(struct NeuralNetwork* nnPtr){
    for(int iWidth=0;iWidth<nnPtr->nbNBL[0];iWidth++){
        nnPtr->inputA[iWidth]=activationF(nnPtr->input[iWidth]);
    }
    matricesMult(nnPtr->w1,nnPtr->input,nnPtr->nbNBL[1],nnPtr->nbNBL[0],1,nnPtr->hiddenLayer);
    matricesCAdd(nnPtr->hiddenLayer,nnPtr->b1,nnPtr->nbNBL[1]);
    for(int iWidth=0;iWidth<nnPtr->nbNBL[1];iWidth++){
        nnPtr->hiddenLayerA[iWidth]=activationF(nnPtr->hiddenLayer[iWidth]);
    }
    matricesMult(nnPtr->w2,nnPtr->hiddenLayerA,nnPtr->nbNBL[2],nnPtr->nbNBL[1],1,nnPtr->outputLayer);
    matricesCAdd(nnPtr->outputLayer,nnPtr->b2,nnPtr->nbNBL[2]);
    for(int iWidth=0;iWidth<nnPtr->nbNBL[2];iWidth++){
        nnPtr->outputLayerA[iWidth]=activationF(nnPtr->outputLayer[iWidth]);
    }
}

//  load an save nn
/*void saveNn(char* fileName){
    f=fopen(fileName,"w");
    if(f == NULL) {
		printf("file can't be opened\n");
		exit(1);
	}
}
void loadNn(char* fileName){
     f=fopen(fileName,"r");
     if(f == NULL) {
		printf("file can't be opened\n");
		exit(1);
	}
    fprintf(f,"%f\n", 0);
}*/
// Predict
/*double* predict(double* input,char* fileName){
    struct NeuralNetwork* nnPtr = initNn(nbNBL,input);
    loadNn(fileName);
    feedForward(nnPtr);
    free(nnPtr);
    return nnPtr->outputLayerA;
}*/


