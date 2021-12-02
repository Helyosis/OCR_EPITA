#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#include <SDL_image.h>
#include <err.h>
#pragma GCC diagnostic pop

#include "Pixels.h"


// Function : transpose
// Description : Transpose a matrix
// input : float *matrix the matrix
//         int width the width of the matrix
//         int height the height of the matrix
// return : float * the transposed matrix

float *transpose(float *matrix, int width, int height)
{
    int *transposed = malloc(sizeof(int) * width * height);
    if (transposed == NULL)
        err(1, "Transpose : Malloc failed");
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            transposed[i * height + j] = matrix[j * width + i];
        }
    }
    return transposed;
}

// Function : inverse_matrix
// Description : Inverse a matrix using Gauss-Jordan elimination
// input : float **matrix the matrix
//         int order the order of the matrix
// return : void

// Function to perform the inverse operation on the matrix.
void InverseOfMatrix(float** matrix, int order)
{
    // Matrix Declaration.
 
    float temp;
 
    // PrintMatrix function to print the element
    // of the matrix.
 
    // Create the augmented matrix
    // Add the identity matrix
    // of order at the end of original matrix.
    for (int i = 0; i < order; i++) {
 
        for (int j = 0; j < 2 * order; j++) {
 
            // Add '1' at the diagonal places of
            // the matrix to create a identity matrix
            if (j == (i + order))
                matrix[i][j] = 1;
        }
    }
 
    // Interchange the row of matrix,
    // interchanging of row will start from the last row
    for (int i = order - 1; i > 0; i--) {
 
        // Swapping each and every element of the two rows
        // if (matrix[i - 1][0] < matrix[i][0])
        // for (int j = 0; j < 2 * order; j++) {
        //
        //        // Swapping of the row, if above
        //        // condition satisfied.
        // temp = matrix[i][j];
        // matrix[i][j] = matrix[i - 1][j];
        // matrix[i - 1][j] = temp;
        //    }
 
        // Directly swapping the rows using pointers saves
        // time
 
        if (matrix[i - 1][0] < matrix[i][0]) {
            float* temp = matrix[i];
            matrix[i] = matrix[i - 1];
            matrix[i - 1] = temp;
        }
    }

 
    // Replace a row by sum of itself and a
    // constant multiple of another row of the matrix
    for (int i = 0; i < order; i++) {
 
        for (int j = 0; j < order; j++) {
 
            if (j != i) {
 
                temp = matrix[j][i] / matrix[i][i];
                for (int k = 0; k < 2 * order; k++) {
 
                    matrix[j][k] -= matrix[i][k] * temp;
                }
            }
        }
    }
 
    // Multiply each row by a nonzero integer.
    // Divide row element by the diagonal element
    for (int i = 0; i < order; i++) {
 
        temp = matrix[i][i];
        for (int j = 0; j < 2 * order; j++) {
 
            matrix[i][j] = matrix[i][j] / temp;
        }
    }
 
    return;
}

// Function : maltiply_matrix
// Description : Multiply two matrices
// input : float **matrix1 the first matrix
//         float **matrix2 the second matrix
//         int col1 the number of columns of the first matrix
//         int row1 the number of rows of the first matrix
//         int col2 the number of columns of the second matrix
//         int row2 the number of rows of the second matrix
// return : float ** the multiplied matrix

float **multiply_matrix(float **matrix1, float **matrix2, int col1, int row1, int col2, int row2)
{
    float **result = malloc(sizeof(float *) * row1 * col2);
    if (result == NULL)
        err(1, "Multiply : Malloc failed");
    for (int i = 0; i < row1; i++)
    {
        for (int j = 0; j < col2; j++)
        {
            result[i * col2 + j] = 0;
            for (int k = 0; k < col1; k++)
            {
                result[i * col2 + j] += matrix1[i * col1 + k] * matrix2[k * col2 + j];
            }
        }
    }
    return result;
}




// Function : homographic_transformation
// Description : Apply a homographic transformation to an image
// input : SDL_Surface *image the image to transform
//         float *matrix the matrix to apply
//         int width the width of the image
//         int height the height of the image
// return : SDL_Surface * the transformed image

SDL_S
