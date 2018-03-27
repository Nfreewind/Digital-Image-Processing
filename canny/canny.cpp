/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : canny.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *			
 * Reference               : https://docs.opencv.org/3.3.1/da/d5c/tutorial_canny_detector.html
 * 				https://docs.opencv.org/3.3.1/dd/d1a/group__imgproc__feature.html#ga04723e007ed888ddf11d9ba04e2232de
 * 				http://www.cnblogs.com/love6tao/p/5152020.html
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.24, William Yu
                             ver1.1, 2018.03.24, William Yu, add notes
=====================================================================================*/
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
//-----------------------------------【全局变量声明部分】--------------------------------------
//		描述：全局变量的声明
//-----------------------------------------------------------------------------------------------
using namespace cv;
Mat src, src_gray;
Mat dst, detected_edges;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 200;
int ratio = 3;
int const max_ratio = 12;
int kernel_size = 3;
const char* window_name = "Edge Map";

//--------------------------------【 CannyThreshold()函数 】----------------------------------------------
//        参数说明：
//            static void CannyThreshold(int, void*)
//			回调
//-------------------------------------------------------------------------------------------------
static void CannyThreshold(int, void*)
{
    blur( src_gray, detected_edges, Size(3,3) );
    //--------------------------------【 Canny()函数 】----------------------------------------------
    //        参数说明：
    //            void cv::Canny(InputArray image,OutputArray edges,double threshold1,double threshold2,int apertureSize = 3,bool L2gradient = false )
    //			Parameters
    //				image : 8-bit input image.
    //				edges : output edge map; single channels 8-bit image, which has the same size as image .
    //				threshold1 : first threshold for the hysteresis procedure.
    //                          threshold2 : second threshold for the hysteresis procedure.
    //				apertureSize : aperture size for the Sobel operator.
    //				L2gradient : a flag, indicating whether a more accurate L2 norm =√‾‾((dI/dx)^2+(dI/dy)^2)‾‾‾ should be used to calculate the image gradient magnitude ( L2gradient=true ), or whether the default L1 norm =|dI/dx|+|dI/dy| is enough ( L2gradient=false ).
    //-------------------------------------------------------------------------------------------------
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
}
////////////////////【注意】此处比较有趣的想法,当不能直接处理彩色图片时，先处理其灰度图，再叠加回彩色图片上面去，满分操作///////////

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


//--------------------------------【 myCanny()函数 】----------------------------------------------
//	Canny算子求边缘点具体算法步骤如下：
//			1. 用高斯滤波器平滑图像
//			2. 用一阶偏导有限差分计算梯度幅值和方向
//			3. 对梯度幅值进行非极大值抑制
//			4. 用双阈值算法检测和连接边缘        
//	参数说明：
//            void myCanny()
//			参数 : 
//			参数 : 
//			参数 :
//-------------------------------------------------------------------------------------------------
void myCanny( InputArray image, OutputArray edges, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false )
{
    //高斯平滑滤波
    myGaussianBlur();
  ////
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}


static void help()
{
    cout << "\n这是一个演示数字图像处理canny算子检测边缘的程序"
            "\n使用OpenCV version " << CV_VERSION << endl;
    cout << "\n使用:\n"
            "\t./canny [图片名称 -- 默认值为 ubuntu.png]\n\n";
	
    cout << "\nThis is a demo of ,"
            "\nUsing OpenCV version " << CV_VERSION << endl;
    cout << "\nCall:\n"
            "    ./canny [image_name -- Default is ubuntu.png]\n\n";
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
    //Mat src;
    src= imread(filename, 1);
    if(src.empty()) {printf("Cannot read image file: %s\n", filename.c_str()); return -1; }
    
    //Mat dst;
    dst.create( src.size(), src.type() );
    //Mat src_gray;
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
  
    //--------------------------------【 CannyThreshold()函数 】----------------------------------------------
    //         opencv内置的canny算子,并做阈值调控
    //-------------------------------------------------------------------------------------------------
    namedWindow( window_name, WINDOW_AUTOSIZE );
    createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
    createTrackbar( "ratio:", window_name, &ratio, max_ratio, CannyThreshold );
    CannyThreshold(0, 0);
    
    //--------------------------------【 myCanny()函数 】----------------------------------------------
    //         自己编写的canny算子
    //--------------------------------------------------------------------------------------
    Mat dst_myCanny;
    dst_myCanny.create(src.size(), src.type());
    myCanny(src_gray, dst_myCanny, 13, 69, 3); 

    cvNamedWindow("GaussImage",0);  
    cvShowImage("GaussImage",pCanny);               //显示高斯图
    cvWaitKey(0);  
    cvDestroyWindow("GaussImage"); 
    
    
    
    //namedWindow("dst_myCanny", WINDOW_AUTOSIZE);
    //imshow("dst_myCanny", dst_myCanny);

    
    namedWindow("src", WINDOW_AUTOSIZE);
    imshow("src", src);
    
    waitKey(0);
    return 0;
       
}
