#include <stdio.h>
#include <stdlib.h>
#include <math.h>

srand(time(NULL));
//double* layer;
struct NeuralNetwork* initNn(){
    int nbLayer=2;
    int input[2]={0,0};// This is only for testing uses 
    int nbNBL[3]={2,2,1};// Number of neurone by layer
    double* w1   = calloc(nbNBL[1]*nbNBL[0],sizeof(double));// Do not forget to free height*width
    double* w2  = calloc(nbNBL[2]*nbNBL[1],sizeof(double));
    double* b1  = calloc(nbNBL[1],sizeof(double));
    double* b2  = calloc(nbNBL[2],sizeof(double));
    double* hiddenLayer  = calloc(nbNBL[1],sizeof(double));
    double* hiddenLayerA = calloc(nbNBL[1],sizeof(double));
    double* outputLayer   = calloc(nbNBL[2],sizeof(double));
    double* outputLayerA  = calloc(nbNBL[2],sizeof(double));
    struct NeuralNetwork* nnPtr = malloc(sizeof(NeuralNetwork));
    struct NeuralNetwork nn = *nnPtr;
    nn->nbLayer=nbLayer;
    nn->input=input;
    nn->nbNBL=nbNBL;
    nn->w1=w1;
    nn->w2=w2;
    nn->b1=b1;
    nn->b2=b2;
    nn->outputLayer=outputLayer;
    nn->hiddenLayer=hiddenLayer;
    nn->hiddenLayerA=hiddenLayerA;
    nn->outputLayer=outputLayer;
    nn->outputLayerA=outputLayerA;
    initWB(nnPtr);
    return nnPtr;
}

void initWB(struct NeuralNetwork* nnPtr){
    //initialisation loi uniforme  U[-1/sqrt(n),1/sqrt(n)] n nb neurone du layer d'avant
    for(int iHeight=0;iHeight<nnPtr->nbNBL[0];iHeight++){
        for(int iWidth=0; iWidth<nnPtr->nbNBL[1];iWidth++){
            nnPtr->w1[iHeight*nbNBL[0]+iWidth]=rand();//faire loi uniforme
            nnPtr->b1[iWidth]=rand();
        }
    }
    for(int iHeight=0;iHeight<nnPtr->nbNBL[2];iHeight++){
        for(int iWidth=0; iWidth<nnPtr->nbNBL[1];iWidth++){
            nnPtr->w2[iHeight*nbNBL[1]+iWidth]=rand();//faire loi uniforme
            nnPtr->b2[iWidth]=rand();
        }
    }
}
// Feedforward
void feedForward(struct NeuralNetwork* nnPtr){
    //activation(sum (weight * input +biases))
    matricesMult(nnPtr->w1,nnPtr->input,nnPtr->nbNBL[1],nnPtr->nbNBL[0],1,nnPtr->hiddenLayer);
    matricesCAdd(nnPtr->hiddenLayer,nnPtr->b1,nnPtr->nbNBL[1]);
    for(int iWidth=0;iWidth<nnPtr->nbNBL[1];iWidth++){
        nnPtr->hiddenLayerA[iWidth]=activation(nnPtr->nbNBL[1],nnPtr->hiddenLayer[iWidth],nnPtr->hiddenLayer);
    }
    matricesMult(nnPtr->w2,nnPtr->hiddenLayerA,nnPtr->nbNBL[2],nnPtr->nbNBL[1],1,nnPtr->outputLayer);
    matricesCAdd(nnPtr->ouputLayer,nnPtr->b2,nnPtr->nbNBL[2]);
    for(int iWidth=0;iWidth<nnPtr->nbNBL[2];iWidth++){
        nnPtr->outputLayerA[iWidth]=activation(nnPtr->nbNBL[2],nnPtr->outputLayer[iWidth],nnPtr->outputLayer);
    }
}

// Multiply matA * matB
void matricesMult(double* matA, double* matB,int heightA,int widthA, int widthB, double* result) {
    if(widthA==widthB==1){// MatB is a scalar
        for(int iHeightA=0;iHeightA<heightA;iHeightA++){
            result[iHeightA]=matA[iHeightA]*matB[0];
        }
    }
    else
    {
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
}
// Add matA (column) + matB (column)
void matricesCAdd(double* matA, double *matB, int heightA) {
    for(int iHeight=0;iHeight<nbNBL[1];iHeight++){
        matA[iHeight]+=matB[iHeight];
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
// Activation function: Softmax
double activation(int height, double x, double* mat){
    long double sum=0;
    for(int i=0;i<height;i++){
        sum+=exp(mat[i]);
    }
    return exp(x)/sum;
}
// Derivative of the activation function
double activationDerivative(int height, double x, double* mat){
    return activation(height,x,mat)*(1-activation(height,x,mat));
}

double activationDerivativeL(int height, double* mat){
    double* result=calloc(width,sizeof(double));
    for(int iHeight=0;iHeight<height;iHeight++){
        result[i] = activationDerivative(height,mat[iHeight],mat);
    }
    return result;
}
// Hadamard product
double* hadamardProduct(double* matA, double* matB,int width, int height){
    double* result  = calloc(width*height,sizeof(double));//DO NOT FORGET TO FREE
    for(int iHeight=0;iWidth<height;iHeight++){
        for(int iWidth=0; iWidth<width;iWidth++){
            result[iHeight*width+iHeight] = matA[iHeight*width+iWidth] * matB[iHeight*width+iWidth];
        }
    }
    return result;
}
// Gradient descent, m is the number of training sample
void gradientDescent(struct NeuralNetwork* nnPtr,double* nablaB2, double* nablaW2, double* nablaB1, double* nablaW1, int m){
    double stepSize=0.01;
    double* w1old=nnPtr->w1;//TODO pb ça ne copie pas? si je modifie ça modifie l'original?
    double* b1old=nnPtr->b1;
    double* w2old=nnPtr->w2;
    double* b2old=nnPtr->b2;
    //TODO vérifier tout les index ex: nablaW1
    for(int iHeight=0;iHeight<nnPtr->nbNBL[0];iHeight++){
        for(int iWidth=0; iWidth<nnPtr->nbNBL[1];iWidth++){
            nnPtr->w1[iHeight*nbNBL[0]+iWidth]=w1old[iHeight*nbNBL[0]+iWidth]-stepSize*((1/m)*nablaW1[iHeight]);
            nnPtr->b1[iWidth]=b1old[iWidth]-stepSize*((1/m)*nablaB1[iWidth]);
        }
    }
    for(int iHeight=0;iHeight<nnPtr->nbNBL[2];iHeight++){
        for(int iWidth=0; iWidth<nnPtr->nbNBL[1];iWidth++){
            nnPtr->w2[iHeight*nbNBL[1]+iWidth]=w2old[iHeight*nbNBL[0]+iWidth]-stepSize*((1/m)*nablaW2[iHeight]);
            nnPtr->b2[iWidth]=b2old[iWidth]-stepSize*((1/m)*nablaB2[iWidth]);
        }
    }
}

// Back propagation 
void backPropagation(struct NeuralNetwork* nnPtr, double* targetOutput, double* nablaB2, double* nablaW2, double* nablaB1, double* nablaW1){
    // Calculate the output error
    int outputHeight=nnPtr->nbNBL[2];
    double* adO = activationDerivativeL(outputHeight,nnPtr->outputLayer);//ouputLayer zl al=s(zl)
    double* cd = costDerivative(targetOutput,nnPtr->outputLayerA,outputHeight);
    double* errorOutputLayer = hadamardProduct(cd,ad,1,outputHeight);
    free(cd);
    free(adO);
    //TODO Faire le calcule des nabla dans une autre fonction
    nablaB2 = errorOutputLayer;                      
    matricesMult(errorOutputLayer,nnPtr->hiddenLayerA,nnPtr->nbNBL[1],outputHeight,nablaW2);//we should transpose hiddenLayerA but it's a column mat, so we do nothing
    // Back propagate the error to the hidden layer
    int hiddenLHeight=nnPtr->nbNBL[1];
    double* adH = activationDerivative(hiddenLHeight,nnPtr->hiddenLayer);
    double* r=calloc(3,sizeof(double));
    double* wT=calloc(nnPtr->nbNBL[1],sizeof(double));
    matTranspose(nnPtr->w2,wT,nnPtr->nbNBL[1],nnPtr->nbNBL[2]);
    matricesMult(wT,errorOutputLayer,nnPtr->nbNBL[1],nnPtr->nbNBL[2],1,r);
    double* errorHiddenLayer = hadamardProduct(cd,adH,1,hiddenLHeight);
    free(adH);
    free(wT);
    free(r);
    nablaB1 = errorHiddenLayer;//nablaB1=δ(l)
    matricesMult(errorHiddenLayer,,,,nablaW1); // h(l=0) activation de l'input layer??? nablaW1=h(l-1)*δ(l)
    free(errorHiddenLayer);//ERROR? for example nablaB1 copy the double* or point to double*
    free(errorOutputLayer);//ERROR?
}

double* costDerivative(double* targetOutput, double* output, int height){
    double* result = calloc(width,sizeof(double));//DO not forget to free
    for(int iHeight=0;iHeight<height;iHeight++){
        result[iHeight] = output[iHeight] - targetOutput[iHeight];
    }
    return result;
}
// Train the neural network
/*
void trainNn(){
    //test------------------------------------
    double* targetOutput=calloc(1,sizeof(double));
    targetOutput[0]=1;
    //----------------------------------------
    double* nablaB2;
    double* nablaW2;
    double* nablaB1;
    double* nablaW1;
    struct NeuralNetwork* nnPtr= initNn();
    initWB(nnPtr);
    while(iteration<iterationLimit){
        nablaB2=nablaW2=nablaB1=nablaW1=[0,...];
        for(iSample<nbSample){
            feedforward(nnPtr);
	        backPropagation(nnPtr);
	        GD();
            //average cost
        }
    }
}
*/