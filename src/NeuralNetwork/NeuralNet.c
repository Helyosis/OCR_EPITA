#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#pragma GCC diagnostic pop
#include "NeuralNetInit.h"
#include "MatUtils.h"

// Activation function: sigmoid
double sigmoidF(double x){
    return 1/(1+exp(-x));
}

void sigmoid(double* mA, double* m, int width){
    for(int iWidth=0;iWidth<width;iWidth++){
        mA[iWidth]=sigmoidF(m[iWidth]);
    }
}

void softMax(double* mA, double* m, int width){
    double sum=0;
    for(int iWidth=0;iWidth<width;iWidth++){
        sum+=exp(m[iWidth]);
    } 
    for(int iWidth=0;iWidth<width;iWidth++){
        mA[iWidth]=exp(m[iWidth])/sum;
    }
}

// Feedforward: z(l)=w(l)*a(l)+b(l), a(l)=f(z(l))
void feedForward(struct NeuralNetwork* nnPtr){
    int d=nnPtr->nbNBL[0];
    int l=nnPtr->nbNBL[1];
    int k=nnPtr->nbNBL[2];
    matMult(nnPtr->input,nnPtr->wh,1,d,l,nnPtr->h);
    matAdd(nnPtr->h,nnPtr->bh,1,l);
    sigmoid(nnPtr->hA, nnPtr->h,l);
    matMult(nnPtr->hA,nnPtr->wy,1,l,k,nnPtr->y);
    matAdd(nnPtr->y,nnPtr->by,1,k);
    softMax(nnPtr->yA,nnPtr->y,k);
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
    fprintf(fptr,"%d\n%d\n%d\n", nnPtr->nbNBL[0], nnPtr->nbNBL[1],
    nnPtr->nbNBL[2]);
    int inputH=nnPtr->nbNBL[0];
    int hiddenH=nnPtr->nbNBL[1];
    int outputH=nnPtr->nbNBL[2];
    writeMat(inputH,hiddenH,fptr,nnPtr->wh);
    writeMat(1,hiddenH,fptr,nnPtr->bh);
    writeMat(hiddenH,outputH,fptr,nnPtr->wy);
    writeMat(1,outputH,fptr,nnPtr->by);
    fclose(fptr);
}

/*void loadMat(int height, int width,FILE* fptr,double* l){
    for (int iHeight = 0; iHeight < height; iHeight++) {
        for (int iWidth = 0; iWidth < width; iWidth++) {
            fscanf(fptr,"%f",l[iHeight*width+iWidth]);
        }
    }
}

void loadNn(char* fileName, struct NeuralNetwork* nnPtr){
    FILE *fptr;
    fptr=fopen(fileName,"r");
    if(fptr == NULL) {
		printf("file can't be opened\n");
		exit(1);
	}
    double i=0;
    int* nbNBL = calloc(3, sizeof(int));
    fscanf (fptr, "%d", &nbNBL[0]);
    fscanf (fptr, "%d", &nbNBL[1]);
    fscanf (fptr, "%d", &nbNBL[2]);
    while (!feof(fptr))
    {  
        fscanf(fptr, "%f", &i);
    }
    fclose(fptr);
}

// Predict
int predict(double* input, char* fileName){
    
    struct NeuralNetwork* nnPtr = initNn(nbNBL, input);
    loadNn(fileName, nnPtr);
    feedForward(nnPtr);
    free(nbNBL);
    free(nnPtr);
    double max=0;
    int nb=0;
    for(int i=;;)
    return nb;
}*/


