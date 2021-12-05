#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL.h>
#include <SDL_image.h>
#include <err.h>
#include <stdio.h>
#pragma GCC diagnostic pop

#include "Pixels.h"
#include "OrderPoints.h"
#include "../Verbose.h"
#include "../NeuralNetwork/MatUtils.h"
#define FILL(mat, x, y) mat[x * 8 + y]




// Function : Fill_matrix
// Description : Computes the value for the homographic transphormation matrix
// input : rderedPoints *points the points

// return : **float the matrix
double *Fill_matrix(orderedPoints points, int size)
{
    double *mat = calloc(8*8, sizeof(double));
    //1st ROW
    FILL(mat, 0, 2) = 1;
    FILL(mat, 1, 5) = 1;
    //2nd ROW
    FILL(mat, 2, 0) = size;
    FILL(mat, 2, 2) = 1;
    FILL(mat, 2, 6) = -points.ur.x*size;
    FILL(mat, 3,3) = size;
    FILL(mat, 3,5) = 1;
    FILL(mat, 3,6) = -points.ur.y*size;
    //3rd ROW
    FILL(mat, 4,1) = size;
    FILL(mat, 4,2) = 1;
    FILL(mat, 4,7) = -points.ll.x*size;
    FILL(mat, 5,4) = size;
    FILL(mat, 5,5) = 1;
    FILL(mat, 5,7) = -points.ll.y*size;
    //4th ROW
    FILL(mat, 6,0) = size;
    FILL(mat, 6,1) = size;
    FILL(mat, 6,2) = 1;
    FILL(mat, 6,6) = -points.lr.x*size;
    FILL(mat, 6,7) = -points.lr.y*size;
    FILL(mat, 7,3) = size;
    FILL(mat, 7,4) = size;
    FILL(mat, 7,5) = 1;
    FILL(mat, 7,6) = -points.lr.x*size;
    FILL(mat, 7,7) = -points.lr.y*size;

    double *res = calloc(8,sizeof(double));
    double b[] = {
        (double)points.ul.x,
        (double)points.ul.y,
        (double)points.ur.x,
        (double)points.ur.y,
        (double)points.ll.x,
        (double)points.ll.y,
        (double)points.lr.x,
        (double)points.lr.y
    };

    // Create a Matrix to store the transpose of the matrix mat
    double *mat_transpose = calloc(8*8,sizeof(double));
    matTranspose(mat,mat_transpose, 8,8);
    // Create a Matrix to store the multiplication of the matrix mat_transpose and mat
    double *res_mult = calloc(8*8,sizeof(double));
    matricesMult(mat_transpose, mat, 8,8,8, res_mult);
    // Free mat we don't need it anymore
    free(mat);
    double *res_inv = calloc(8*8,sizeof(double));
    if (!inverse(res_mult,res_inv,8)){
        errx(1,"Matrix is singular");
    }
    
    
    free(res_mult);
    double *res_mult2 = calloc(8*8,sizeof(double));
    matricesMult(res_inv, mat_transpose, 8,8,8, res_mult2);
    free(res_inv);
    free(mat_transpose);
    matricesMult(res_mult2, b, 8,8,1, res);
    free(res_mult2);
    return res;
}


// Function : HomographicTransphorm
// Description : Computes the homographic transphormation of an image
// input : SDL_Surface *src the source image
//         orderedPoints points the points
// return : SDL_Surface *the image after the transphormation

SDL_Surface *HomographicTransform(SDL_Surface *src, orderedPoints points,int size)
{
    // Create a matrix to store the homographic transphormation matrix
    double *mat = Fill_matrix(points, size);
    
    //double xOff = src->w/2;
    //double yOff = src->h/2;
    // Create a matrix to store the transphormation matrix
    SDL_Surface *dest = SDL_CreateRGBSurface(0, size, size, 32, 0, 0, 0, 0);
    // Create a matrix to store the transphormation matrix
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            double x = (mat[0] * i + mat[1] * j + mat[2])/(mat[6] * i + mat[7] * j + 1);
            double y = (mat[3] * i + mat[4] * j + mat[5])/(mat[6] * i + mat[7] * j + 1);

            if (x >= 0 && x < src->w && y >= 0 && y < src->h)
            {
                Uint32 pixel = getPixel(src, x, y);
                putPixel(dest, i, j, pixel);
            }
            
        }
    }
    free(mat);
    return dest;
}