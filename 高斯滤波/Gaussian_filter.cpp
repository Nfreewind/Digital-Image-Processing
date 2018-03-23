/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : Gaussian_filter.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *			
 * Reference               : https://docs.opencv.org/3.3.1/d4/d86/group__imgproc__filter.html#gaabe8c836e97159a9193fb0b11ac52cf1
 *				https://docs.opencv.org/3.3.1/d4/d86/group__imgproc__filter.html#gac05a120c1ae92a6060dd0db190a61afa
 * 				https://docs.opencv.org/3.3.1/de/db2/laplace_8cpp-example.html#a17
 *				http://blog.csdn.net/weixin_37720172/article/details/72843238,,,该文档代码怀疑有错误,已与作者联系
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.22, William Yu
                             ver1.1, 2018.03.22, William Yu, add notes
=====================================================================================*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


//--------------------------------【 mygetGaussianKernel()函数 】----------------------------------------------
//         参数说明：
//            double ** mygetGaussianKernel( int ksize, double sigma)
//			参数 ksize: 
//			参数 sigma: 
//-------------------------------------------------------------------------------------------------
double ** mygetGaussianKernel(int ksize, double sigma)
{
    int size= ksize;
    int i, j;  
    double sum = 0.0;  
    int center = size; //以第一个点的坐标为原点，求出中心点的坐标  
  
    double **arr = new double*[size];//建立一个size*size大小的二维数组  
    for (i = 0; i < size; ++i)  
        arr[i] = new double[size];  
      
    for (i = 0; i < size; ++i)  
        for (j = 0; j < size; ++j) 
	{  
            arr[i][j] = exp(-((i - center)*(i - center) + (j - center)*(j - center)) / (sigma*sigma * 2));  
            sum += arr[i][j];  
        }  
    for (i = 0; i < size; ++i)  
        for (j = 0; j < size; ++j)  
            arr[i][j] /= sum;  
    return arr;
}




//--------------------------------【 myGaussianBlur()函数 】----------------------------------------------
//        参数说明：
//            void myGaussianBlur(Mat& srcImage, Mat& dst_myGaussianBlur, int ksize, int sigma)
//			参数 & srcImage: 
//			参数 & dst_myGaussianBlur: 
//			参数 ksize:
//			参数 sigma:
//-------------------------------------------------------------------------------------------------    
void myGaussianBlur(const Mat& srcImage, Mat& dst_myGaussianBlur, int ksize, int sigma)
{
    if(srcImage.empty()) {printf("Cannot read image file"); return; }
    //--------------------------------【 mygetGaussianKernel()函数 】----------------------------------------------
    //		获取高斯滤波器系数. 
    //		Returns Gaussian filter coefficients.
    //-------------------------------------------------------------------------------------------------
    double **arr; 
    arr = mygetGaussianKernel(ksize, sigma); 
    
    
    Mat tmp(srcImage.size(), srcImage.type());  
    for (int i = 0; i < srcImage.rows; ++i)  
        for (int j = 0; j < srcImage.cols; ++j) 
	{
            //边缘不进行处理  
            if ((i - 1) > 0 && (i + 1) < srcImage.rows && (j - 1) > 0 && (j + 1) < srcImage.cols) 
	    { 
                tmp.at<Vec3b>(i, j)[0] = 0;
                tmp.at<Vec3b>(i, j)[1] = 0;
                tmp.at<Vec3b>(i, j)[2] = 0;
		/////////////////////////////////////////////////[??????????????]未完成!这里使用的是3X3模板啊!!!
                for (int x = 0; x < ksize; ++x)
		{  
                    for (int y = 0; y < ksize; ++y)
		    {  
                            tmp.at<Vec3b>(i, j)[0] += arr[x][y] * srcImage.at<Vec3b>(i + 1 - x, j + 1 - y)[0];  
                            tmp.at<Vec3b>(i, j)[1] += arr[x][y] * srcImage.at<Vec3b>(i + 1 - x, j + 1 - y)[1];  
                            tmp.at<Vec3b>(i, j)[2] += arr[x][y] * srcImage.at<Vec3b>(i + 1 - x, j + 1 - y)[2];  
                    }  
                }  
            }  
        }  
    tmp.copyTo(dst_myGaussianBlur);  
    
}


static void help()
{
    cout << "\n这是一个演示数字图像处理获取高斯滤波器,以及进行高斯平滑滤波的程序"
            "\n使用OpenCV version " << CV_VERSION << endl;
    cout << "\n使用:\n"
            "\t./Gaussian_filter [图片名称 -- 默认值为 ubuntu.png]\n\n";
	
    cout << "\nThis is a demo of ,"
            "\nUsing OpenCV version " << CV_VERSION << endl;
    cout << "\nCall:\n"
            "    ./Gaussian_filter [image_name -- Default is ubuntu.png]\n\n";
}

const char* keys =
{
    "{help h||}{@image |ubuntu.png|input image name}"
};


int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, keys); 
    if (parser.has("help"))  { help();  return 0; }

    string filename = parser.get<string>(0);

    Mat srcImage;
    srcImage= imread(filename, 1);
    if(srcImage.empty()) {printf("Cannot read image file: %s\n", filename.c_str()); return -1; }
    
    Mat dst_myGaussianBlur;
    dst_myGaussianBlur.create(srcImage.size(), srcImage.type());
    
    Mat dst_opencvGaussianBlur;
    dst_opencvGaussianBlur.create(srcImage.size(), srcImage.type());

    //--------------------------------【 myGaussianBlur()函数 】----------------------------------------------
    //		高斯平滑滤波.
    //		Blurs an image using a Gaussian filter.
    //-------------------------------------------------------------------------------------------------
    int sigma= 3;
    int ksize= 15;
    myGaussianBlur(srcImage, dst_myGaussianBlur, ksize, sigma);
    //保证安全,归一化到0~255  
    normalize(dst_myGaussianBlur, dst_myGaussianBlur, 0, 255, CV_MINMAX);

    //--------------------------------【 GaussianBlur()函数 】----------------------------------------------
    //         opencv内置的高斯平滑滤波函数
    //-------------------------------------------------------------------------------------------------
    int sigmax = 3;
    int sigmay = 3;
    int ksizew = 15;
    int ksizeh = 15;
    //int ksizew = (sigmax*5)|1;
    //int ksizeh = (sigmay*5)|1;
    //namedWindow( "Sigma", 0 );
    //createTrackbar( "Sigma", "Sigma", &sigma, 15, 0 );
    GaussianBlur(srcImage, dst_opencvGaussianBlur, Size(ksizew, ksizeh), sigmax, sigmay);
    
    
    namedWindow("srcImage", WINDOW_AUTOSIZE);
    imshow("srcImage", srcImage);
    namedWindow("dst_myGaussianBlur",WINDOW_AUTOSIZE);
    imshow("dst_myGaussianBlur",dst_myGaussianBlur);
    namedWindow("dst_opencvGaussianBlur", WINDOW_AUTOSIZE);
    imshow("dst_opencvGaussianBlur", dst_opencvGaussianBlur);
    waitKey(0);
    return 0;
}
