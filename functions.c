#include "functions.h"

kernelType Kx = {{-1.0,0.0,1.0},{-2.0,0.0,2.0},{-1.0,0.0,1.0}};
kernelType Ky = {{1.0,2.0,1.0},{0.0,0.0,0.0},{-1.0,-2.0,-1.0}};


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

void swap_coordinates(kernelType* kernel,uint8_t row1,uint8_t col1,uint8_t row2,uint8_t col2)
{
	double holder = (*kernel)[row1][col1];
	(*kernel)[row1][col1] = (*kernel)[row2][col2];
	(*kernel)[row2][col2] = holder;
}


void invert_kernel(kernelType* kernel) //it is stupid
{
	//for KERNEL SIZE = 3
	swap_coordinates(kernel, 0, 1, 1, 0);
	swap_coordinates(kernel, 0, 2, 2, 0);
	swap_coordinates(kernel, 1, 2, 2, 1);
	fprintf(stderr,"kernel:\n");
	for(uint8_t i=0;i<KERNEL_SIZE;++i)
	{
		for(uint8_t j=0;j<KERNEL_SIZE;++j)
			fprintf(stderr,"%lf ",(*kernel)[i][j]);
		fprintf(stderr,"\n");
	}
}

void conv2_calculate_element(imgType* img,kernelType* kernel,uint16_t row_start,uint16_t col_start,double* res)
{
	for(uint8_t i=0;i<KERNEL_SIZE;++i)
	{
		for(uint8_t j=0;j<KERNEL_SIZE;++j)
		{
			*res+=(*kernel)[i][j]*((*img)[row_start+i][col_start+j]);
		}
	}

}

void conv2(imgType* img,kernelType* kernel,imgType* img_proc)
{
	invert_kernel(kernel);
	 for(uint16_t i=2;i<RES_HEIGHT;++i)
	{
		for(uint16_t j=2;j<RES_WIDTH;++j)
		{

	 		fprintf(stderr,"calculating element: %u %u\r",i,j);	
	 		conv2_calculate_element(img, kernel, i-(KERNEL_SIZE-1), j-(KERNEL_SIZE-1),&((*img_proc)[i-(KERNEL_SIZE-1)][j-(KERNEL_SIZE-1)]));
		}
	}
	fprintf(stderr,"\n");
	fprintf(stderr,"sample value conv2: %lf\n",(*img_proc)[100][100]);
}

void gradient(imgType* img,imgType* grad,imgType* theta)
{
	fprintf(stderr,"gradient start\n");
	imgType x_res,y_res;
	fprintf(stderr,"conv2 for Kx\n");
	conv2(img,&Kx,&x_res);
	fprintf(stderr,"conv2 for Ky\n");
	conv2(img,&Ky,&y_res);

	 fprintf(stderr,"grad and theta\n");
	 for(uint16_t i=0;i<RES_HEIGHT;++i)
	{
		for(uint16_t j=0;j<RES_WIDTH;++j)
		{
	 		fprintf(stderr,"calculating theta and gradient for: %u %u\r",i,j);	
	 		(*grad)[i][j] = hypot(x_res[i][j],y_res[i][j]);
	 		(*theta)[i][j] = atan2(x_res[i][j],y_res[i][j]);
		}
	}
	fprintf(stderr,"\n");
}



void top(imgType* img,imgType* grad,imgType* theta)
{
	//generate_gauss(gauss);
	gradient(img,grad,theta);
}
