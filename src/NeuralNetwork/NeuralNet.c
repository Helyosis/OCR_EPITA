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

// Feedforward: z(l)=w(l)*a(l)+b(l), a(l)=f(z(l))
void feedForward(struct NeuralNetwork* nnPtr){
    matricesMult(nnPtr->w1,nnPtr->input,nnPtr->nbNBL[1],nnPtr->nbNBL[0],1,nnPtr->hiddenLayer);
    matricesCSub(nnPtr->hiddenLayer,nnPtr->b1,nnPtr->nbNBL[1]);
    for(int iWidth=0;iWidth<nnPtr->nbNBL[1];iWidth++){
        nnPtr->hiddenLayerA[iWidth]=activationF(nnPtr->hiddenLayer[iWidth]);
    }
    matricesMult(nnPtr->w2,nnPtr->hiddenLayerA,nnPtr->nbNBL[2],nnPtr->nbNBL[1],1,nnPtr->outputLayer);
    matricesCSub(nnPtr->outputLayer,nnPtr->b2,nnPtr->nbNBL[2]);
    for(int iWidth=0;iWidth<nnPtr->nbNBL[2];iWidth++){
        nnPtr->outputLayerA[iWidth]=activationF(nnPtr->outputLayer[iWidth]);
    }
}

void writeMat(int height, int width,FILE* fptr,double* l){
    for (int iHeight = 0; iHeight < height; iHeight++) {
        for (int iWidth = 0; iWidth < width; iWidth++) {
            fprintf(fptr,"%f\n",l[iHeight*width+iWidth]);
        }
    }
}
//  load an save nn
void saveNn(char* fileName, struct NeuralNetwork* nnPtr){
    FILE* fptr;
    fptr=fopen(fileName,"w");
    if(fptr == NULL) {
		printf("file can't be opened\n");
		exit(1);
	}
    fprintf(fptr,"%d\n%d\n%d\n",nnPtr->nbNBL[0],nnPtr->nbNBL[1],
    nnPtr->nbNBL[2]);
    int inputH=nnPtr->nbNBL[0];
    int hiddenH=nnPtr->nbNBL[1];
    int outputH=nnPtr->nbNBL[2];
    writeMat(hiddenH,inputH,fptr,nnPtr->w1);
    writeMat(hiddenH,1,fptr,nnPtr->b1);
    writeMat(outputH,hiddenH,fptr,nnPtr->w2);
    writeMat(outputH,1,fptr,nnPtr->b2);
    fclose(fptr);
}

/*void loadNn(char* fileName,struct NeuralNetwork* nnPtr){
    FILE *fptr;
    fptr=fopen(fileName,"r");
    if(fptr == NULL) {
		printf("file can't be opened\n");
		exit(1);
	}
    double i=0;
    fscanf (fptr, "%d", &i); 
    while (!feof(fptr))
    {  
        fscanf(fptr, "%f", &i);
    }
    nnPtr->nbNBL;
    nnPtr->w1;
    nnPtr->w2;
    nnPtr->b1;
    nnPtr->b2;
    fclose(fptr);
}*/
// Predict
/*double* predict(double* input,char* fileName){
    int* nbNBL = calloc(3, sizeof(3));
    nbNBL[0]  =  2; nbNBL[1]  =  2; nbNBL[2]  =  1;
    struct NeuralNetwork* nnPtr = initNn(nbNBL,input);
    loadNn(fileName,nnPtr);
    feedForward(nnPtr);
    free(nnPtr);
    return nnPtr->outputLayerA;
}*/


