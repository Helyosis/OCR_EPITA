#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#pragma GCC diagnostic pop

#include "LoadDataSet.h"
#include "NeuralNetInit.h"
#include "MatUtils.h"
#include "../Verbose.h"

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
		error_s("Cannot open %s. No such file or directory.", fileName);
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

void loadMat(int height, int width,FILE* fptr,double* l){
    for (int iHeight = 0; iHeight < height; iHeight++) {
        for (int iWidth = 0; iWidth < width; iWidth++) {
            fscanf(fptr,"%lf",&l[iHeight*width+iWidth]);
        }
    }
}

struct NeuralNetwork* loadNn(char* fileName){
    FILE *fptr;
    fptr=fopen(fileName,"r");
    if(fptr == NULL) {
        error_s("Cannot open %s. No such file or directory.", fileName);
	}
    int* nbNBL = calloc(3, sizeof(int));
    fscanf (fptr, "%d", &nbNBL[0]);
    fscanf (fptr, "%d", &nbNBL[1]);
    fscanf (fptr, "%d", &nbNBL[2]);
    struct NeuralNetwork* nnPtr =  initNn(nbNBL, NULL);
    loadMat(nnPtr->nbNBL[0], nnPtr->nbNBL[1],fptr,nnPtr->wh);
    loadMat(1, nnPtr->nbNBL[1],fptr,nnPtr->bh);
    loadMat(nnPtr->nbNBL[1], nnPtr->nbNBL[2],fptr,nnPtr->wy);
    loadMat(1, nnPtr->nbNBL[2],fptr,nnPtr->by);
    fclose(fptr);
    return nnPtr;
}

// Predict
double* predict(char* imageFileName, char* SavefileName){
    struct tImage* img=loadImg(imageFileName);
    struct NeuralNetwork* nnPtr=loadNn(SavefileName);
    nnPtr->input=img->pixVect;
    feedForward(nnPtr);
    double* res=malloc(nnPtr->nbNBL[2]*sizeof(double));
    for (int i = 0; i  < nnPtr->nbNBL[2]; ++i) {
        res[i] = nnPtr->yA[i];
    }
    freeNn(nnPtr);
    free(nnPtr);
    return res;
}


