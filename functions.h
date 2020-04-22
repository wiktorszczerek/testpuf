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


static double Kx[KERNEL_SIZE][KERNEL_SIZE] = {{-1.0,0.0,1.0},{-2.0,0.0,2.0},{-1.0,0.0,1.0}};
static double Ky[KERNEL_SIZE][KERNEL_SIZE] = {{1.0,2.0,1.0},{0.0,0.0,0.0},{-1.0,-2.0,-1.0}};


void generate_gauss(double arr[GAUSS_SIZE][GAUSS_SIZE]);
void swap_coordinates(double kernel[KERNEL_SIZE][KERNEL_SIZE],uint8_t row1,uint8_t col1,uint8_t row2,uint8_t col2);
void invert_kernel(double kernel[KERNEL_SIZE][KERNEL_SIZE]);
void conv2_calculate_element(double img[RES_HEIGHT][RES_WIDTH],double kernel[KERNEL_SIZE][KERNEL_SIZE],uint8_t row_start,uint8_t col_start,double res);
void conv2(double img[RES_HEIGHT][RES_WIDTH],double kernel[KERNEL_SIZE][KERNEL_SIZE],double img_proc[RES_HEIGHT][RES_WIDTH]);
void gradient(double img[RES_HEIGHT][RES_WIDTH],double grad[RES_HEIGHT][RES_WIDTH],double theta[RES_HEIGHT][RES_WIDTH]);

void top(double img[RES_HEIGHT][RES_WIDTH],double grad[RES_HEIGHT][RES_WIDTH],double theta[RES_HEIGHT][RES_WIDTH]);

#endif
