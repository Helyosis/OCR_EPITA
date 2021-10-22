#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "NeuralNet.h"
// Only for testing uses
//--------------------------------------------
void printMat(double* mat, int height, int width){
    for (int iHeight = 0; iHeight < height; iHeight++) {
        for (int iWidth = 0; iWidth < width; iWidth++) {
            printf("%f ,",mat[iHeight * width + iWidth]);
        }
        printf("\n");
    }
}
//------------------------------------------
//put this in utils----------------------------------------------------------
double randomDouble(){
    #ifndef RANDOMSEED
    #define RANDOMSEED srand(time(NULL));
    #endif
    double x = (double)rand()/(double)(RAND_MAX);
    return x;
}
// Multiply matA * matB
void matricesMult(double* matA, double* matB,int heightA,int widthA, int widthB, double* result) {
    for(int iHeightA=0;iHeightA<heightA;iHeightA++){
        for(int iWidthB=0;iWidthB<widthB;iWidthB++){
            double sum=0;
            for(int iWidthA=0;iWidthA<widthA;iWidthA++){
                sum = sum + matA[iHeightA*widthA+iWidthA] * matB[iWidthA*widthB+iWidthB];
            }
            result[iHeightA*widthB+iWidthB] = sum;
        }   
    } 
}
// Add matA (column) + matB (column)
void matricesCAdd(double* matA, double* matB, int heightA) {
    for(int iHeight=0;iHeight<heightA;iHeight++){
        matA[iHeight]+=matB[iHeight];
    }
} 
// Add matA + matB
void matricesAdd(double* matA, double* matB, int height,int width){
    for(int iHeight = 0; iHeight < height; ++iHeight)
	{
		for(int iWidth=0; iWidth < width; ++iWidth)
		{
			matA[iHeight*width+iWidth] = matA[iHeight*width+iWidth] + matB[iHeight*width+iWidth];
		}
	}
}
// Tranpose a mat
void matTranspose(double* mat, double* result, int height, int width){
    for(int iHeight = 0; iHeight<height*width; iHeight++) {
        int i = iHeight/height;
        int j = iHeight%height;
        result[iHeight] = mat[width*j + i];
    }
}
//------------------------------------------------------------------
// Initialize the weight with uniform distribution and the bias to 0
void initWB(struct NeuralNetwork* nnPtr) {
    printf("initW1B1\n");
    printf("pointer nnPtr (initWB)%p\n",nnPtr);
    printf("pointer nbNBL (initWB)%p\n",nnPtr->nbNBL);
    printf("pointer w1 (initWB)%p\n",nnPtr->w1);
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[1]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[0]; iWidth++) {
            nnPtr->w1[iHeight * nnPtr->nbNBL[0] + iWidth] = randomDouble()/sqrt(nnPtr->nbNBL[0]);
            nnPtr->b1[iHeight] = 0;
        }
    }
    printf("initW2B2\n");
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[2]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[1]; iWidth++) {
            nnPtr->w2[iHeight * nnPtr->nbNBL[1] + iWidth] = randomDouble()/sqrt(nnPtr->nbNBL[1]);
            nnPtr->b2[iHeight] = 0;
        }
    }
}

struct NeuralNetwork* initNn(int* nbNBL,double* input){
    struct NeuralNetwork* nnPtr = malloc(sizeof(struct NeuralNetwork));
    
    nnPtr->input = input;
    nnPtr->inputA = calloc(nbNBL[0],sizeof(double));
    //nnPtr->nbNBL = nbNBL;// Number of neurone by layer
    nnPtr->nbNBL = nbNBL;
    nnPtr->w1 = calloc(nnPtr->nbNBL[1]*nnPtr->nbNBL[0],sizeof(double));// Do not forget to free height*width
    nnPtr->w2 = calloc(nnPtr->nbNBL[2]*nnPtr->nbNBL[1],sizeof(double));

    nnPtr->b1 = calloc(nnPtr->nbNBL[1],sizeof(double));
    nnPtr->b2 = calloc(nnPtr->nbNBL[2],sizeof(double));

    nnPtr->outputLayer = calloc(nnPtr->nbNBL[2],sizeof(double));
    nnPtr->hiddenLayer = calloc(nnPtr->nbNBL[1],sizeof(double));

    nnPtr->hiddenLayerA = calloc(nnPtr->nbNBL[1],sizeof(double));
    nnPtr->outputLayerA = calloc(nnPtr->nbNBL[2],sizeof(double));

    nnPtr->nablaB2=calloc(nbNBL[2],sizeof(double));
    nnPtr->nablaW2=calloc(nbNBL[2]*nbNBL[1],sizeof(double));
    nnPtr->nablaB1=calloc(nbNBL[1],sizeof(double));
    nnPtr->nablaW1=calloc(nbNBL[1]*nbNBL[0],sizeof(double));

    initWB(nnPtr);
    return nnPtr;
}

// Activation function: sigmoid
double activationF(double x){
    /*double sum=0;
    for(int i=0;i<height;i++){
        sum+=exp(mat[i]);
    }
    return exp(x)/sum;*/
    return 1/(1+exp(-x));
}
// Derivative of the activation function
double activationFDerivative(double x){
    //return activationF(height,x,mat)*(1-activationF(height,x,mat)); faux
    return activationF(x)*(1-activationF(x));
}

double* activationFDerivativeL(int height, double* mat){
    double* result=calloc(height,sizeof(double));
    for(int iHeight=0;iHeight<height;iHeight++){
        result[iHeight] = activationFDerivative(mat[iHeight]);
    }
    return result;
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
    matricesCAdd(nnPtr->outputLayer,nnPtr->b2,nnPtr->nbNBL[2]);//TODO verifier renvoi un scalaire
    for(int iWidth=0;iWidth<nnPtr->nbNBL[2];iWidth++){
        nnPtr->outputLayerA[iWidth]=activationF(nnPtr->outputLayer[iWidth]);
    }
}

// Hadamard product
double* hadamardProduct(double* matA, double* matB,int width, int height){
    double* result  = calloc(width*height,sizeof(double));//DO NOT FORGET TO FREE
    for(int iHeight=0;iHeight<height;iHeight++){
        for(int iWidth=0; iWidth<width;iWidth++){
            result[iHeight*width+iHeight] = matA[iHeight*width+iWidth] * matB[iHeight*width+iWidth];
        }
    }
    return result;
}
// Gradient descent, m is the number of training sample
void gradientDescent(struct NeuralNetwork* nnPtr, double m){
    double stepSize=0.01;
    //TODO vérifier tout les index ex: nablaW1
    for(int iHeight=0;iHeight<nnPtr->nbNBL[1];iHeight++){
        for(int iWidth=0; iWidth<nnPtr->nbNBL[0];iWidth++){
            nnPtr->w1[iHeight*nnPtr->nbNBL[0]+iWidth]-=stepSize*((1/m)*nnPtr->nablaW1[iHeight]);
            nnPtr->b1[iWidth]-=stepSize*((1/m)*nnPtr->nablaB1[iWidth]);
            printf("gd w1: %f\n",stepSize*((1/m)*nnPtr->nablaW1[iHeight]));
        }
    }
    for(int iHeight=0;iHeight<nnPtr->nbNBL[2];iHeight++){
        for(int iWidth=0; iWidth<nnPtr->nbNBL[1];iWidth++){
            nnPtr->w2[iHeight*nnPtr->nbNBL[1]+iWidth]-=stepSize*((1/m)*nnPtr->nablaW2[iHeight]);
            nnPtr->b2[iWidth]-=stepSize*((1/m)*nnPtr->nablaB2[iWidth]);
        }
    }
}
// Derivative of the cost
double* costDerivative(double* targetOutput, double* output, int height){
    double* result = calloc(height,sizeof(double));//DO not forget to free
    for(int iHeight=0;iHeight<height;iHeight++){
        result[iHeight] = output[iHeight] - targetOutput[iHeight];
    }
    printf("costDerivative: \n");
    printMat(result,height,1);
    return result;
}
// Back propagation 
void backPropagation(struct NeuralNetwork* nnPtr, double* targetOutput){
    // Calculate the output error
    int outputHeight=nnPtr->nbNBL[2];
    double* adO = activationFDerivativeL(outputHeight,nnPtr->outputLayer);
    double* cd = costDerivative(targetOutput,nnPtr->outputLayerA,outputHeight);

    printf("errorOutputL : ");
    printMat(adO,nnPtr->nbNBL[2],1); 
    printf("cd : ");
    printMat(cd,nnPtr->nbNBL[2],1);    

    double* errorOutputLayer = hadamardProduct(cd,adO,1,outputHeight);
    free(adO);
    int hiddenLHeight=nnPtr->nbNBL[1];
    double* r=calloc(outputHeight*hiddenLHeight,sizeof(double));
    nnPtr->nablaB2 = errorOutputLayer; //TODO faire une seconde struct  

    printf("errorOutputL : ");
    printMat(errorOutputLayer,nnPtr->nbNBL[2],1);  

    matricesMult(errorOutputLayer,nnPtr->hiddenLayerA,outputHeight,1,hiddenLHeight,nnPtr->nablaW2);// matTranspose(hiddenLayerA)
    //matricesAdd(nablaW2,r,outputHeight,hiddenLHeight);
    free(r);
    // Back propagate the error to the hidden layer
    double* adH = activationFDerivativeL(hiddenLHeight,nnPtr->hiddenLayer);
    r = calloc(hiddenLHeight*1,sizeof(double));
    double* wT = calloc(hiddenLHeight*outputHeight,sizeof(double));
    matTranspose(nnPtr->w2,wT,outputHeight,hiddenLHeight);
    matricesMult(wT,errorOutputLayer,hiddenLHeight,outputHeight,1,r);
    double* errorHiddenLayer = hadamardProduct(r,adH,1,hiddenLHeight);
    free(cd); free(adH); free(wT);
    int inputLHeight=nnPtr->nbNBL[0];
    //r=calloc(hiddenLHeight*inputLHeight,sizeof(double));
    nnPtr->nablaB1 = errorHiddenLayer;//erro use mat add???

    printf("errorHiddenL :\n");
    printMat(errorHiddenLayer,nnPtr->nbNBL[2],1);  

    matricesMult(errorHiddenLayer,nnPtr->inputA,hiddenLHeight,1,inputLHeight,nnPtr->nablaW1);// matTranspose(inputA)
    //matricesAdd(nablaW1,r,hiddenLHeight,inputLHeight);//erro use mat add???
    free(r);
    printf("nB2:\n");
    printMat(nnPtr->nablaB2,nnPtr->nbNBL[2],1);
    printf("nB1:\n");
    printMat(nnPtr->nablaB1,nnPtr->nbNBL[1],1);
    printf("nW2:\n");
    printMat(nnPtr->nablaW2,nnPtr->nbNBL[2],nnPtr->nbNBL[1]);
    printf("nW1:\n");
    printMat(nnPtr->nablaW1,nnPtr->nbNBL[1],nnPtr->nbNBL[0]);
}
//  load an save nn
/*void saveNn(char* fileName){

}
void loadNn(char* fileName){

}*/
// Predict
/*double* predict(double* input,char* fileName){
    struct NeuralNetwork* nnPtr = initNn(nbNBL,input);
    loadNn(fileName);
    feedForward(nnPtr);
    free(nnPtr);
    return nnPtr->outputLayerA;
}*/

void initNablaWB(double* nablaB2, double* nablaW2, double* nablaB1, double* nablaW1,int* nbNBL){
    for (int iHeight = 0; iHeight < nbNBL[1]; iHeight++) {
        for (int iWidth = 0; iWidth < nbNBL[0]; iWidth++) {
            nablaW1[iHeight * nbNBL[0] + iWidth] = 0;
            nablaB1[iHeight] = 0;
        }
    }
    for (int iHeight = 0; iHeight < nbNBL[2]; iHeight++) {
        for (int iWidth = 0; iWidth < nbNBL[1]; iWidth++) {
            nablaW2[iHeight * nbNBL[1] + iWidth] = 0;
            nablaB2[iHeight] = 0;
        }
    }
}
// Train the neural network
void trainNn(int iterationLimit){
    // This is only for testing uses 
    //----------------------------------------
    double* targetOutput=calloc(1,sizeof(double));
    targetOutput[0]=1;
    int* nbNBL=calloc(3, sizeof(3));
    nbNBL[0] = 2; nbNBL[1] = 2; nbNBL[2] = 1;
    double* input=calloc(2, sizeof(int));
    input[0] = 0; input[1] = 1;
    printf("trainNN nbNBN %p\n",nbNBL);
    //----------------------------------------
    printf("initNN\n");
    struct NeuralNetwork* nnPtr= initNn(nbNBL,input);
    printf("initWB\n");
    //initWB(nnPtr);
    int iterationNum=0;
    int nbSample=1;
    while(iterationNum<iterationLimit){
        //initNablaWB(nablaB2,nablaW2,nablaB1,nablaW1,nbNBL);
        for(int iSample=0;iSample<nbSample;iSample++){
            printf("New sample\n");
            printMat(nnPtr->hiddenLayerA,nnPtr->nbNBL[1],1);
            printMat(nnPtr->outputLayerA,nnPtr->nbNBL[2],1);
            feedForward(nnPtr);
            printf("feedForward\n");
            printMat(nnPtr->hiddenLayerA,nnPtr->nbNBL[1],1);
            printMat(nnPtr->outputLayerA,nnPtr->nbNBL[2],1);
	        backPropagation(nnPtr,targetOutput);
            printf("backPropagation\n");
            printMat(nnPtr->hiddenLayerA,nnPtr->nbNBL[1],1);
            printMat(nnPtr->outputLayerA,nnPtr->nbNBL[2],1);
	        gradientDescent(nnPtr,nbSample);
            printf("gradienDescent\n");
            printMat(nnPtr->hiddenLayerA,nnPtr->nbNBL[1],1);
            printf("result:\n");
            printMat(nnPtr->outputLayerA,nnPtr->nbNBL[2],1);
            //average cost?
        }
        iterationNum+=1;
    }
    free(targetOutput); free(nbNBL); free(input);
    //free(nnPtr);
}

