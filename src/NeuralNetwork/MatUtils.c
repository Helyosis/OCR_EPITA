#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <stdio.h>
#include <stdlib.h>
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
// Substract matA (column) + matB (column)
void matricesCSub(double* matA, double* matB, int heightA) {
    for(int iHeight=0;iHeight<heightA;iHeight++){
        matA[iHeight]-=matB[iHeight];
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

// Hadamard product: result[i][j] = matA[i][j] * matB[i][j]
double* hadamardProduct(double* matA, double* matB,int width, int height){
    double* result  = calloc(width*height,sizeof(double));
    for(int iHeight=0;iHeight<height;iHeight++){
        for(int iWidth=0; iWidth<width;iWidth++){
            result[iHeight*width+iWidth] = matA[iHeight*width+iWidth] * matB[iHeight*width+iWidth];
        }
    }
    return result;
}



// Function : swap_rows
// Description : Swap two rows of a matrix
// input : double *matrix the matrix
//         int cols the number of columns
//         int row1 the first row to swap
//         int row2 the second row to swap
// return : void

void swap_rows(double *matrix, int cols,int row1, int row2){
    double temp;
    for(int i = 0; i < cols; i++){
        temp = matrix[row1 * cols + i];
        matrix[row1 * cols + i] = matrix[row2 * cols + i];
        matrix[row2 * cols + i] = temp;
    }
}

// Function : inverse_matrix
// Description : Inverse a matrix using Gauss-Jordan elimination
// input : double *matrix the matrix
//         int order the order of the matrix
// return : void

void inverse_matrix(double *matrix, int rows, int cols){
    double temp;

    for(int i = 0; i < rows; i++){
        for(int j = rows; j < cols; j++){
            if((j-rows) == i){
                matrix[i*cols+j] = 1;
            }
        }
    }   
    for(int i = rows - 1; i > 0; i--){
        if(matrix[i-1] < matrix[i]){
            swap_rows(matrix, cols, i, i-1);
        }
    }
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
            if(i != j){
                temp = matrix[j*cols+i] / matrix[i*cols+i];
                for(int k = 0; k < cols; k++){
                    matrix[j*cols+k] -= matrix[i*cols+k] * temp;
                }
            }
        }
    }
    for(int i = 0; i < rows; i++){
        temp = matrix[i * cols + i];
        for(int j = 0; j < cols; j++){
            matrix[i * cols + j] = matrix[i * cols + j] / temp;
        }
    }
}


