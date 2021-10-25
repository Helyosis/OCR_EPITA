#include "NeuralNet.h"
#include "NeuralNetInit.h"
#include "MatUtils.h"
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>//TODO
// Derivative of the activation function: sigmoid
double activationFDerivative(double x){
    return activationF(x)*(1-activationF(x));
}

double* activationFDerivativeL(int height, double* mat){
    double* result=calloc(height,sizeof(double));
    for(int iHeight=0;iHeight<height;iHeight++){
        result[iHeight] = activationFDerivative(mat[iHeight]);
    }
    return result;
}

// Gradient descent, m is the number of training sample
// New weight = old weight-(stepSize/m)*(nabla of the weight)
// New bias   = old bias-(stepSize/m)*(nabla of the bias)
void gradientDescent(struct NeuralNetwork* nnPtr, double m){
    double stepSize=0.25;
    for(int iHeight=0;iHeight<nnPtr->nbNBL[1];iHeight++){
        for(int iWidth=0; iWidth<nnPtr->nbNBL[0];iWidth++){
            nnPtr->w1[iHeight*nnPtr->nbNBL[0]+iWidth]+= -stepSize*((1/m)*nnPtr->nablaW1[iHeight]);
            nnPtr->b1[iHeight]+= -stepSize*((1/m)*nnPtr->nablaB1[iHeight]);
        }
    }
    for(int iHeight=0;iHeight<nnPtr->nbNBL[2];iHeight++){
        for(int iWidth=0; iWidth<nnPtr->nbNBL[1];iWidth++){
            nnPtr->w2[iHeight*nnPtr->nbNBL[1]+iWidth]+= -stepSize*((1/m)*nnPtr->nablaW2[iHeight]);
            nnPtr->b2[iHeight]+= -stepSize*((1/m)*nnPtr->nablaB2[iHeight]);
        }
    }
}

// Derivative of the cost: y-predicted output
double* costDerivative(double* targetOutput, double* output, int height){
    double* result = calloc(height,sizeof(double));
    for(int iHeight=0;iHeight<height;iHeight++){
        result[iHeight] = output[iHeight] - targetOutput[iHeight];
    }
    return result;
}
// Calculate the output error: (gradient of the cost function)°f'(z(l))
double* calculateErrorOutputLayer(struct NeuralNetwork* nnPtr, double* targetOutput){ 
    int outputHeight=nnPtr->nbNBL[2];
    double* adO = activationFDerivativeL(outputHeight,nnPtr->outputLayer);
    double* cd = costDerivative(targetOutput,nnPtr->outputLayerA,outputHeight);  
    double* errorOutputLayer = hadamardProduct(cd,adO,1,outputHeight);
    free(adO);
    free(cd); 
    return errorOutputLayer;
}
// Propagate the output error to the hidden layer: (weight(l+1)T*errorOutputlayer)°f'(z(l))
double* calculateErrorHiddenLayer(struct NeuralNetwork* nnPtr, double* errorOutputLayer){
    int hiddenLHeight=nnPtr->nbNBL[1];
    int outputHeight=nnPtr->nbNBL[2];
    double* adH = activationFDerivativeL(hiddenLHeight,nnPtr->hiddenLayer);
    double* r = calloc(hiddenLHeight*1,sizeof(double));
    double* wT = calloc(hiddenLHeight*outputHeight,sizeof(double));
    matTranspose(nnPtr->w2,wT,outputHeight,hiddenLHeight);
    matricesMult(wT,errorOutputLayer,hiddenLHeight,outputHeight,1,r);
    double* errorHiddenLayer = hadamardProduct(r,adH,1,hiddenLHeight);
    free(adH); 
    free(wT);
    free(r);
    return errorHiddenLayer;
}
// Back propagation: for each layer propagate the error of the predicted output
void backPropagation(struct NeuralNetwork* nnPtr, double* targetOutput){
    int outputHeight=nnPtr->nbNBL[2];
    int hiddenLHeight=nnPtr->nbNBL[1];
    int inputLHeight=nnPtr->nbNBL[0];
    double* errorOutputLayer = calculateErrorOutputLayer(nnPtr,targetOutput);
    // Calculate nabla B2 and nabla W2
    // Nabla of a bias(l) += error(l)
    // Nabla of a weight(l) += error(l)*a(l-1)T
    matricesCAdd(nnPtr->nablaB2,errorOutputLayer,outputHeight);
    double* r=calloc(outputHeight*hiddenLHeight,sizeof(double));  
    matricesMult(errorOutputLayer,nnPtr->hiddenLayerA,outputHeight,1,hiddenLHeight,r);
    matricesAdd(nnPtr->nablaW2,r,outputHeight,hiddenLHeight);
    free(r);
    // Back propagate the error to the hidden layer
    double* errorHiddenLayer=calculateErrorHiddenLayer(nnPtr,errorOutputLayer);
    // Update nabla B1 and nabla W1
    matricesCAdd(nnPtr->nablaB1,errorHiddenLayer,hiddenLHeight);
    r = calloc(hiddenLHeight*inputLHeight,sizeof(double));
    matricesMult(errorHiddenLayer,nnPtr->inputA,hiddenLHeight,1,inputLHeight,r);
    matricesAdd(nnPtr->nablaW1,r,hiddenLHeight,inputLHeight);
    free(errorHiddenLayer);
    free(r);
    free(errorOutputLayer);
}

// Creat sample: two input 0 or 1 and the target output
double* creatSample(int nbSample){
    double* sampleList=calloc(nbSample*3,sizeof(double));
    for(int i=0;i<nbSample*3;i+=3){
        int i1=rand()  % 2;
        int i2=rand() % 2;
        sampleList[i] = i1;
        sampleList[i+1] = i2;
        sampleList[i+2] = (i1^i2);
    }
    return sampleList;
}
// Initialize nablaW=0 and nablaB=0
void initNablaWB(struct NeuralNetwork* nnPtr){
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[1]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[0]; iWidth++) {
            nnPtr->nablaW1[iHeight * nnPtr->nbNBL[0] + iWidth] = 0;
            nnPtr->nablaB1[iHeight] = 0;
        }
    }
    for (int iHeight = 0; iHeight < nnPtr->nbNBL[2]; iHeight++) {
        for (int iWidth = 0; iWidth < nnPtr->nbNBL[1]; iWidth++) {
            nnPtr->nablaW2[iHeight * nnPtr->nbNBL[1] + iWidth] = 0;
            nnPtr->nablaB2[iHeight] = 0;
        }
    }
}
// Train the neural network
void trainNn(int iterationLimit){
    int nbSample=325;
    double* targetOutput=calloc(1,sizeof(double));
    double* sampleList=creatSample(nbSample);
    int* nbNBL=calloc(3, sizeof(3));
    nbNBL[0] = 2; nbNBL[1] = 2; nbNBL[2] = 1;
    double* input=calloc(2, sizeof(double));
    input[0] = sampleList[0];
    input[1] = sampleList[1];
    targetOutput[0]=sampleList[2];
    struct NeuralNetwork* nnPtr= initNn(nbNBL,input);
    int iterationNum=0;
   
    while(iterationNum<iterationLimit){

        double success=0;
        initNablaWB(nnPtr);
        for(int iSample=0;iSample<nbSample*3;iSample+=3){

            nnPtr->input[0]=sampleList[iSample];
            nnPtr->input[1]=sampleList[iSample+1];
            targetOutput[0]=sampleList[iSample+2];
            feedForward(nnPtr);
	        backPropagation(nnPtr,targetOutput);
            
            if(nnPtr->outputLayerA[0]>0.5){
                if((int)targetOutput[0]==1)
                        success++;
                else
                {
                    if(iterationNum%500==0)
                        initWB(nnPtr);
                }
            }
            else{
                if((int)targetOutput[0]==0)
                        success++;
                else{
                    if(iterationNum%500==0)
                        initWB(nnPtr);
                }
            }
        }
        printf("nw1:\n");
        printMat(nnPtr->nablaW1,nnPtr->nbNBL[1],nnPtr->nbNBL[0]);
        printf("nb1:\n");
        printMat(nnPtr->nablaB1,nnPtr->nbNBL[1],1);
        printf("nW2:\n");
        printMat(nnPtr->nablaW2,nnPtr->nbNBL[2],nnPtr->nbNBL[1]);
        printf("nb2:\n");
        printMat(nnPtr->nablaB2,nnPtr->nbNBL[2],1);
        gradientDescent(nnPtr,nbSample);
        printf("w1:\n");
        printMat(nnPtr->w1,nnPtr->nbNBL[1],nnPtr->nbNBL[0]);
        printf("b1:\n");
        printMat(nnPtr->b1,nnPtr->nbNBL[1],1);
        printf("w2:\n");
        printMat(nnPtr->w2,nnPtr->nbNBL[2],nnPtr->nbNBL[1]);
        printf("b2:\n");
        printMat(nnPtr->b2,nnPtr->nbNBL[2],1);
        printf("MOYENNE: %f \n",(success/nbSample)*100);
        sleep(0.5);
        
        iterationNum+=1;
    }
   
    free(nbNBL);
    free(input);
    free(targetOutput);
    freeNn(nnPtr);
    free(nnPtr);
    free(sampleList);
}