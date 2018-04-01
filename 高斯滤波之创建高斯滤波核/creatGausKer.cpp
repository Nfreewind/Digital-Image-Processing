/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : creatGausKer.cpp
 * Description             : (编译环境：Ubuntu16.04)
 *				生成二维任意大小高斯滤波核
 * Reference               : http://www.cnblogs.com/qiqibaby/p/5289977.html
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.28, William Yu
                             ver1.1, 2018.03.29, William Yu, add notes
                             ver1.1, 2018.03.30, William Yu, 添加 归一化处理--方式二
=====================================================================================*/
#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;


//--------------------------------【 mygetGaussianKernel()函数 】----------------------------------------------
//	double ** mygetGaussianKernel(int ksize, double sigma);
//		参数
//			ksize      :模板大小
//			sigma      :高斯标准差
//------------------------------------------------------------------------------------------------- 
double ** mygetGaussianKernel(int ksize, double sigma)
{
    int i, j;  
    double sum = 0.0;  
    int center = ksize/2; 
    
    ///建立一个size*size大小的动态二维数组  
    double **arr = new double*[ksize];
    for (i = 0; i < ksize; ++i)  
        arr[i] = new double[ksize];  
    
    ///高斯函数  
    for (i = 0; i < ksize; ++i)  
        for (j = 0; j < ksize; ++j) 
	{  
            arr[i][j] = exp(-((i - center)*(i - center) + (j - center)*(j - center)) / (sigma*sigma * 2));  
            sum += arr[i][j];  
        }
        
    ///归一化
    double chacksum=0;
    for (i = 0; i < ksize; ++i)  
        for (j = 0; j < ksize; ++j)  
	{   
	  arr[i][j] /= sum;
	  chacksum += arr[i][j];
	}
    
    ///输出
  
    printf("%.10f ",chacksum);
    for (i = 0; i < ksize; ++i)
      for (j = 0; j < ksize; ++j)
      {
	if ( j%ksize ==0) { printf("\n");}
	printf("%.10f ",arr[i][j]);
      }
    
    return arr;
}

//--------------------------------【 mygetGaussianKernel_2()函数 】----------------------------------------------
//	double * mygetGaussianKernel_2(int ksize, double sigma);
//		参数
//			ksize      :模板大小
//			sigma      :高斯标准差
//------------------------------------------------------------------------------------------------- 

double* mygetGaussianKernel_2(int ksize, double sigma)
{
    int nWindowSize = ksize;
    int nCenter = nWindowSize/2;

    double *pKernel_2 = new double[nWindowSize*nWindowSize];
    double d_sum = 0.0;
    
    
    for(int i=0;i<nWindowSize;i++)
    {
        for (int j=0;j<nWindowSize;j++)
        {
            double n_Disx=i-nCenter;//水平方向距离中心像素距离
            double n_Disy=j-nCenter;
	    
	    ////////////////////////////划重点！！！////////////////////
            pKernel_2[ j*nWindowSize + i ]=
		//本来要除以一个与Sigma相关的系数， 但由于后面要进行归一化处理，所以生成卷积核时，可忽略系数部分
		//exp( -1*(n_Disx*n_Disx+n_Disy*n_Disy)/(2*sigma*sigma) )  	/  ((2.0*3.1415926)*sigma*sigma); 
		exp( -1*(n_Disx*n_Disx+n_Disy*n_Disy)/(2*sigma*sigma) )  ;
            d_sum=d_sum + pKernel_2[ j*nWindowSize+i ];
        }
    }
    
    
    
    //【归一化处理:方式一】模板内部数字之和为近似为1
    double k_sum= 0;
    for(int i=0; i<nWindowSize; i++)
    {
        for (int j=0; j<nWindowSize; j++)
        {
          pKernel_2[j*nWindowSize+i]= pKernel_2[j*nWindowSize+i]/d_sum;
	  k_sum = k_sum + pKernel_2[ j*nWindowSize + i ];
        }
    }
    
    
    /*
    //【归一化处理:方式二】左上角数字归一
    double k_sum= 0;
    double k=pKernel_2[0];
    for(int i=0; i<nWindowSize; i++)
    {
        for (int j=0; j<nWindowSize; j++)
        {
          pKernel_2[j*nWindowSize+i] = pKernel_2[j*nWindowSize+i]/k;
          k_sum += pKernel_2[ j*nWindowSize+i ];
        }
    }
    */
    
    //输出模板
    printf("%.10f ",k_sum);
    for (int i=0; i<nWindowSize*nWindowSize; i++)
    {
        if (i%(nWindowSize)==0) { printf("\n");}
        printf("%.10f ",pKernel_2[i]);
    }
    
    return pKernel_2;
}

int main(int argc, char** argv)
{
    //【方式一】指定模板大小,并由模板大小计算sigma
    //see: https://docs.opencv.org/3.3.1/d4/d86/group__imgproc__filter.html#gac05a120c1ae92a6060dd0db190a61afa
    int ksize= 5;   
    double sigma = 0.3*((ksize-1)*0.5 - 1) + 0.8;
    //【方式二】指定sigma,并由sigma计算模板大小
    //通过sigma得到模板大小 仅需要取均值周围3倍标准差内的值，以外部份直接去掉即可
    //double sigma= 0.7;
    //int ksize = 2* ceil(3*sigma)-1;//ceil函数向上取整，且需要保证为奇数
    
    
    /////使用二维动态数组存放卷积核
    double **arr; 
    arr = mygetGaussianKernel(ksize, sigma); 
     
    printf("\n");
    
    /////使用一维动态数组存放卷积核
    double *arr2; 
    arr2 = mygetGaussianKernel_2(ksize, sigma); 
    
    return 0;
}