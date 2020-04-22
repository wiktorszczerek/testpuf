#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#define GAUSS_SIZE 5
#define SIGMA 1.4
#define NORMAL 1/(2.0*M_PI*SIGMA*SIGMA)
#define RES_WIDTH 256
#define RES_HEIGHT 256
#define KERNEL_SIZE 3

typedef double imgType[RES_HEIGHT][RES_HEIGHT];
typedef double kernelType[KERNEL_SIZE][KERNEL_SIZE];



void generate_gauss(double arr[GAUSS_SIZE][GAUSS_SIZE]);
void swap_coordinates(kernelType* kernel,uint8_t row1,uint8_t col1,uint8_t row2,uint8_t col2);
void invert_kernel(kernelType* kernel);
void conv2_calculate_element(imgType* img,kernelType* kernel,uint16_t row_start,uint16_t col_start,double* res);
void conv2(imgType* img,kernelType* kernel,imgType* img_proc);
void gradient(imgType* img,imgType* grad,imgType* theta);

void top(imgType* img,imgType* grad,imgType* theta);

#endif
