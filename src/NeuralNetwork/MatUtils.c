#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma GCC diagnostic pop

void printMat(double* mat, int height, int width){
    for (int iHeight = 0; iHeight < height; iHeight++) {
        for (int iWidth = 0; iWidth < width; iWidth++) {
            printf(" %4f",mat[iHeight * width + iWidth]);
        }
        printf("\n");
    }
    printf("\n");
}

/* Multiply matA * matB 
@param the height of the matA, the width of matA, the width of matB, the result mat */
void matMult(double* matA, double* matB,int heightA, int widthA, int widthB, double* result) {
    for(int iHeightA=0;iHeightA<heightA;iHeightA++){
        for(int iWidthB=0;iWidthB<widthB;iWidthB++){
            double sum=0;
            for(int iWidthA=0;iWidthA<widthA;iWidthA++){
                sum = sum + matA[iHeightA*widthA+iWidthA] * matB[iWidthA*widthB+iWidthB];
            }
            result[iHeightA*widthB+iWidthB] = sum;
        }   
    }
    /*double sum=0;
    for ( size_t c = 0 ; c < heightA ; c++ )
        {
            for (size_t d = 0 ; d < widthB ; d++ )
            {
                for ( size_t k = 0 ; k < widthA ; k++ )
                {
                    sum = sum + matA[c*widthA+k]*matB[k*widthB+d];
                }
                result[c*widthB+d] = sum;
                sum = 0;
            }
        }*/
}

// Substract: matC = matA - matB
void matSub(double* matA, double* matB,double* matC, int height,int width){
    for(int iHeight = 0; iHeight < height; ++iHeight)
    {
        for(int iWidth=0; iWidth < width; ++iWidth)
        {
            matC[iHeight*width+iWidth] = matA[iHeight*width+iWidth] - matB[iHeight*width+iWidth];
        }
    }
}
// Add: matA = matA + matB
void matAdd(double* matA, double* matB, int height,int width){
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

// Hadamard product: result[i][j] = matA[i][j] * matB[i][j]
double* hadamardProduct(double* matA, double* matB, int height, int width){
    double* result  = calloc(width*height,sizeof(double));
    for(int iHeight=0;iHeight<height;iHeight++){
        for(int iWidth=0; iWidth<width;iWidth++){
            result[iHeight*width+iWidth] = matA[iHeight*width+iWidth] * matB[iHeight*width+iWidth];
        }
    }
    free(matA);
    return result;
}
