#include "functions.h"

void generate_gauss(double arr[GAUSS_SIZE][GAUSS_SIZE])
{
	for(uint8_t i=GAUSS_SIZE-1;i>=0;--i)
	{
		for(uint8_t j=GAUSS_SIZE-1;j>=0;--j)
		{
			arr[i][j] = exp(-((i*i + j*j) / (2.0*SIGMA*SIGMA))) * NORMAL;
		}
	}
}

void swap_coordinates(double kernel[KERNEL_SIZE][KERNEL_SIZE],uint8_t row1,uint8_t col1,uint8_t row2,uint8_t col2)
{
	double holder = kernel[row1][col1];
	kernel[row1][col1] = kernel[row2][col1];
	kernel[row2][col2] = holder;
}


void invert_kernel(double kernel[KERNEL_SIZE][KERNEL_SIZE]) //it is stupid
{
	//for KERNEL SIZE = 3
	swap_coordinates(kernel, 1, 2, 2, 1);
	swap_coordinates(kernel, 1, 3, 3, 1);
	swap_coordinates(kernel, 2, 3, 3, 2);
}

void conv2_calculate_element(double img[RES_HEIGHT][RES_WIDTH],double kernel[KERNEL_SIZE][KERNEL_SIZE],uint8_t row_start,uint8_t col_start,double res)
{
	for(uint8_t i=0;i<KERNEL_SIZE;++i)
	{
		for(uint8_t j=0;j<KERNEL_SIZE;++j)
		{
			res+=kernel[i][j]*img[row_start+i][col_start+j];
		}
	}
}

void conv2(double img[RES_HEIGHT][RES_WIDTH],double kernel[KERNEL_SIZE][KERNEL_SIZE],double img_proc[RES_HEIGHT][RES_WIDTH])
{
	invert_kernel(kernel);

	for(uint8_t i=2;i<RES_HEIGHT;++i)
	{
		for(uint8_t j=2;j<RES_WIDTH;++j)
		{
			conv2_calculate_element(img, kernel, i-(KERNEL_SIZE-1), j-(KERNEL_SIZE-1), img_proc[i][j]);
		}
	}
}

void gradient(double img[RES_HEIGHT][RES_WIDTH],double grad[RES_HEIGHT][RES_WIDTH],double theta[RES_HEIGHT][RES_WIDTH])
{
	double x_res[RES_HEIGHT][RES_WIDTH];
	double y_res[RES_HEIGHT][RES_WIDTH];
	conv2(img,Kx,x_res);
	conv2(img,Ky,y_res);

	for(uint8_t i=0;i<RES_HEIGHT;++i)
	{
		for(uint8_t j=0;j<RES_WIDTH;++j)
		{
			grad[i][j] = hypot(x_res[i][j],y_res[i][j]);
			theta[i][j] = atan2(x_res[i][j],y_res[i][j]);
		}
	}


}



void top(double img[RES_HEIGHT][RES_WIDTH],double grad[RES_HEIGHT][RES_WIDTH],double theta[RES_HEIGHT][RES_WIDTH])
{
	//generate_gauss(gauss);
	gradient(img,grad,theta);
}
