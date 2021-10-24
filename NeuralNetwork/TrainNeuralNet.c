#include "NeuralNet.h"
#include "NeuralNetInit.h"
// Gradient descent, m is the number of training sample
void gradientDescent(struct NeuralNetwork* nnPtr, double m){
    double stepSize=0.01;
    for(int iHeight=0;iHeight<nnPtr->nbNBL[1];iHeight++){
        for(int iWidth=0; iWidth<nnPtr->nbNBL[0];iWidth++){
            nnPtr->w1[iHeight*nnPtr->nbNBL[0]+iWidth]-=stepSize*((1/m)*nnPtr->nablaW1[iHeight]);
            nnPtr->b1[iHeight]-=stepSize*((1/m)*nnPtr->nablaB1[iHeight]);
            printf("gd w1: %f\n",stepSize*((1/m)*nnPtr->nablaW1[iHeight]));
        }
    }
    for(int iHeight=0;iHeight<nnPtr->nbNBL[2];iHeight++){
        for(int iWidth=0; iWidth<nnPtr->nbNBL[1];iWidth++){
            nnPtr->w2[iHeight*nnPtr->nbNBL[1]+iWidth]-=stepSize*((1/m)*nnPtr->nablaW2[iHeight]);
            nnPtr->b2[iHeight]-=stepSize*((1/m)*nnPtr->nablaB2[iHeight]);
        }
    }
}

// Derivative of the cost
double* costDerivative(double* targetOutput, double* output, int height){
    double* result = calloc(height,sizeof(double));
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
    free(nnPtr->nablaB2);
    nnPtr->nablaB2 = errorOutputLayer; //TODO faire une seconde struct  
    printf("errorOutputL : ");
    printMat(errorOutputLayer,nnPtr->nbNBL[2],1);  
    matricesMult(errorOutputLayer,nnPtr->hiddenLayerA,outputHeight,1,hiddenLHeight,nnPtr->nablaW2);// matTranspose(hiddenLayerA)
    free(r);

    // Back propagate the error to the hidden layer
    double* adH = activationFDerivativeL(hiddenLHeight,nnPtr->hiddenLayer);
    r = calloc(hiddenLHeight*1,sizeof(double));
    double* wT = calloc(hiddenLHeight*outputHeight,sizeof(double));
    matTranspose(nnPtr->w2,wT,outputHeight,hiddenLHeight);
    matricesMult(wT,errorOutputLayer,hiddenLHeight,outputHeight,1,r);
    double* errorHiddenLayer = hadamardProduct(r,adH,1,hiddenLHeight);
    free(cd); free(adH); free(wT);free(r);
    int inputLHeight=nnPtr->nbNBL[0];
    //r=calloc(hiddenLHeight*inputLHeight,sizeof(double));
    free(nnPtr->nablaB1);
    nnPtr->nablaB1 = errorHiddenLayer;//erro use mat add???
    printf("errorHiddenL :\n");
    printMat(errorHiddenLayer,nnPtr->nbNBL[2],1);  

    matricesMult(errorHiddenLayer,nnPtr->inputA,hiddenLHeight,1,inputLHeight,nnPtr->nablaW1);// matTranspose(inputA)
    printf("nB2:\n");
    printMat(nnPtr->nablaB2,nnPtr->nbNBL[2],1);
    printf("nB1:\n");
    printMat(nnPtr->nablaB1,nnPtr->nbNBL[1],1);
    printf("nW2:\n");
    printMat(nnPtr->nablaW2,nnPtr->nbNBL[2],nnPtr->nbNBL[1]);
    printf("nW1:\n");
    printMat(nnPtr->nablaW1,nnPtr->nbNBL[1],nnPtr->nbNBL[0]);
}

// Creat sample
double* creatSample(int nbSample){
    double* sampleList=calloc(nbSample*3,sizeof(int));
    printf("creatSample\n");
    for(int i=0;i<nbSample*3-3-3;i+=3){
        double i1=rand();
        double i2=rand();
        sampleList[i]=i1;
        sampleList[i+1]=i2;
        sampleList[i+2]=(double)((int)i1^(int)i2);
    }
    return sampleList;
}

// Train the neural network
void trainNn(int iterationLimit){
    int nbSample=2;
    double* targetOutput=calloc(1,sizeof(double));
    double* sampleList=creatSample(nbSample);
    // This is only for testing uses 
    //----------------------------------------
    /*double* targetOutput=calloc(1,sizeof(double));
    targetOutput[0]=1;*/
    int* nbNBL=calloc(3, sizeof(3));
    nbNBL[0] = 2; nbNBL[1] = 2; nbNBL[2] = 1;
    double* input=calloc(2, sizeof(double));
    input[0] = sampleList[0];
    input[1] = sampleList[1];
    targetOutput[0]=sampleList[2];
    printf("trainNN nbNBN %p\n",nbNBL);
    //----------------------------------------
    printf("initNN\n");
    struct NeuralNetwork* nnPtr= initNn(nbNBL,input);
    int iterationNum=0;
   
    while(iterationNum<iterationLimit){
        for(int iSample=0;iSample<nbSample*3-3;iSample+=3){
            nnPtr->input[0]=sampleList[iSample];
            nnPtr->input[1]=sampleList[iSample+1];
            targetOutput[0]=sampleList[iSample+2];
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
    free(nbNBL);
    free(input);
    free(targetOutput);
    freeNn(nnPtr);
    free(nnPtr);
    free(sampleList);
}