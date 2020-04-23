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

	double max = -1.0;

	 fprintf(stderr,"grad and theta\n");
	 for(uint16_t i=0;i<RES_HEIGHT;++i)
	{
		for(uint16_t j=0;j<RES_WIDTH;++j)
		{
	 		fprintf(stderr,"calculating theta and gradient for: %u %u\r",i,j);	
	 		(*grad)[i][j] = hypot(x_res[i][j],y_res[i][j]);
			if(max<(*grad)[i][j])
			{
				max = (*grad)[i][j];
			}
	 		(*theta)[i][j] = atan2(x_res[i][j],y_res[i][j]);
		}
	}

	for(uint16_t i=0;i<RES_HEIGHT;++i)
	{
		for(uint16_t j=0;j<RES_WIDTH;++j)
		{
			(*grad)[i][j] = ((*grad)[i][j] / max)*255;
		}
	}
	

	fprintf(stderr,"\n");
}

void non_max(imgType* img,imgType* theta,imgType* res)
{
	imgType angle;
	fprintf(stderr,"non_max\n");
	for(uint16_t i=0;i<RES_HEIGHT;++i)
	{
		for(uint16_t j=0;j<RES_WIDTH;++j)
		{
			angle[i][j] = ((*theta)[i][j]*180.0)/M_PI;
			if(angle[i][j]<0)
				angle[i][j]+=180.0;
		}
	}
	double q=255.0,r=255.0;
	for(uint16_t i=1;i<RES_HEIGHT-1;++i)
	{
		for(uint16_t j=1;j<RES_WIDTH-1;++j)
		{
			if((0<= angle[i][j] && angle[i][j]<22.5) || (157.5<=angle[i][j] && angle[i][j] <180.0))
			{
				q=(*img)[i][j+1];
				r=(*img)[i][j-1];
			}
			else if(22.5<=angle[i][j] && angle[i][j]<67.5)
			{
				q = (*img)[i+1][j-1];
				r = (*img)[i-1][j+1];
			}
			else if(67.5<=angle[i][j] && angle[i][j]<112.5)
			{
				q = (*img)[i+1][j];
				r = (*img)[i-1][j];
			}
			else if(112.5<=angle[i][j] && angle[i][j]<157.5)
			{
				q = (*img)[i-1][j-1];
				r = (*img)[i+1][j+1];
			}

			if((*img)[i][j] >= q && (*img)[i][j]>=r)
			{
				(*res)[i][j]=(*img)[i][j];
			}
			else (*res)[i][j] = 0.0;
		}
	}
}

void threshold(imgType* img)
{
	double low = 0.05,high=0.09,weak=25.0,strong =255.0;
	double max = -1.0;

	for(uint16_t i=1;i<RES_HEIGHT-1;++i)
	{
		for(uint16_t j=1;j<RES_WIDTH-1;++j)
		{
			if(max<(*img)[i][j])
				max = (*img)[i][j];
		}
	}

	double high_threshold = max * high;
	double low_threshold = high_threshold * low;

	for(uint16_t i=1;i<RES_HEIGHT-1;++i)
	{
		for(uint16_t j=1;j<RES_WIDTH-1;++j)
		{
			if((*img)[i][j]<low_threshold)
			{
				(*img)[i][j] = 0.0;
			}
			else if((*img)[i][j]>=high_threshold)
			{
				(*img)[i][j] = strong;
			}
			else if(low_threshold<=(*img)[i][j] && (*img)[i][j]<high_threshold)
			{
				(*img)[i][j] = weak;
			}
		}
	}
}

void hysteresis(imgType* img,double weak,double strong)
{
	for(uint16_t i=1;i<RES_HEIGHT-1;++i)
	{
		for(uint16_t j=1;j<RES_WIDTH-1;j++)
		{
			if((*img)[i][j] == weak)
			{
				if(((*img)[i+1][j-1] == strong) || ((*img)[i+1][j] == strong) || ((*img)[i+1][j+1] == strong)
                        || ((*img)[i][j-1] == strong) || ((*img)[i][j+1] == strong)
                        || ((*img)[i-1][j-1] == strong) || ((*img)[i-1][j] == strong) || ((*img)[i-1][j+1] == strong))
				{
					(*img)[i][j] = strong;
				}
				else
				{
					(*img)[i][j] = 0.0;
				}
			}
		}
	}
}


void top(imgType* img,imgType* grad,imgType* theta)
{
	//generate_gauss(gauss);
	gradient(img,grad,theta);
	non_max(grad,theta,img);
	threshold(img);
	hysteresis(img,25.0,255.0);
}
