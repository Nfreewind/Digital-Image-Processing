/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved��CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       ��Ȩ�������У���ѭCC(creativecommons.org)BY-NC-SAЭ����Ȩ��ʽʹ��
 *
 * Filename                : canny.cpp
 * Description             : (���뻷����OpenCV3.3.1 + Ubuntu16.04)
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
//-----------------------------------��ȫ�ֱ����������֡�--------------------------------------
//		������ȫ�ֱ���������
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

//--------------------------------�� CannyThreshold()���� ��----------------------------------------------
//        ����˵����
//            static void CannyThreshold(int, void*)
//			�ص�
//-------------------------------------------------------------------------------------------------
static void CannyThreshold(int, void*)
{
    blur( src_gray, detected_edges, Size(3,3) );
    //--------------------------------�� Canny()���� ��----------------------------------------------
    //        ����˵����
    //            void cv::Canny(InputArray image,OutputArray edges,double threshold1,double threshold2,int apertureSize = 3,bool L2gradient = false )
    //			Parameters
    //				image : 8-bit input image.
    //				edges : output edge map; single channels 8-bit image, which has the same size as image .
    //				threshold1 : first threshold for the hysteresis procedure.
    //                          threshold2 : second threshold for the hysteresis procedure.
    //				apertureSize : aperture size for the Sobel operator.
    //				L2gradient : a flag, indicating whether a more accurate L2 norm =�́6�2�6�2((dI/dx)^2+(dI/dy)^2)�6�2�6�2�6�2 should be used to calculate the image gradient magnitude ( L2gradient=true ), or whether the default L1 norm =|dI/dx|+|dI/dy| is enough ( L2gradient=false ).
    //-------------------------------------------------------------------------------------------------
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
}
////////////////////��ע�⡿�˴��Ƚ���Ȥ���뷨,������ֱ�Ӵ����ɫͼƬʱ���ȴ�����Ҷ�ͼ���ٵ��ӻز�ɫͼƬ����ȥ�����ֲ���///////////

//--------------------------------�� mygetGaussianKernel()���� ��----------------------------------------------
//         ����˵����
//            double ** mygetGaussianKernel( int ksize, double sigma)
//			���� ksize: 
//			���� sigma: 
//-------------------------------------------------------------------------------------------------
double ** mygetGaussianKernel(int ksize, double sigma)
{
    int size= ksize;
    int i, j;  
    double sum = 0.0;  
    int center = size; //�Ե�һ���������Ϊԭ�㣬������ĵ������  
  
    double **arr = new double*[size];//����һ��size*size��С�Ķ�ά����  
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
//--------------------------------�� myGaussianBlur()���� ��----------------------------------------------
//        ����˵����
//            void myGaussianBlur(Mat& srcImage, Mat& dst_myGaussianBlur, int ksize, int sigma)
//			���� & srcImage: 
//			���� & dst_myGaussianBlur: 
//			���� ksize:
//			���� sigma:
//-------------------------------------------------------------------------------------------------    
void myGaussianBlur(const Mat& srcImage, Mat& dst_myGaussianBlur, int ksize, int sigma)
{
    if(srcImage.empty()) {printf("Cannot read image file"); return; }
    //--------------------------------�� mygetGaussianKernel()���� ��----------------------------------------------
    //		��ȡ��˹�˲���ϵ��. 
    //		Returns Gaussian filter coefficients.
    //-------------------------------------------------------------------------------------------------
    double **arr; 
    arr = mygetGaussianKernel(ksize, sigma); 
    
    
    Mat tmp(srcImage.size(), srcImage.type());  
    for (int i = 0; i < srcImage.rows; ++i)  
        for (int j = 0; j < srcImage.cols; ++j) 
	{
            //��Ե�����д���  
            if ((i - 1) > 0 && (i + 1) < srcImage.rows && (j - 1) > 0 && (j + 1) < srcImage.cols) 
	    { 
                tmp.at<Vec3b>(i, j)[0] = 0;
                tmp.at<Vec3b>(i, j)[1] = 0;
                tmp.at<Vec3b>(i, j)[2] = 0;
		/////////////////////////////////////////////////[??????????????]δ���!����ʹ�õ���3X3ģ�尡!!!
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


//--------------------------------�� myCanny()���� ��----------------------------------------------
//	Canny�������Ե������㷨�������£�
//			1. �ø�˹�˲���ƽ��ͼ��
//			2. ��һ��ƫ�����޲�ּ����ݶȷ�ֵ�ͷ���
//			3. ���ݶȷ�ֵ���зǼ���ֵ����
//			4. ��˫��ֵ�㷨�������ӱ�Ե        
//	����˵����
//            void myCanny()
//			���� : 
//			���� : 
//			���� :
//-------------------------------------------------------------------------------------------------
void myCanny( InputArray image, OutputArray edges, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false )
{
    //��˹ƽ���˲�
    myGaussianBlur();
  ////
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}


static void help()
{
    cout << "\n����һ����ʾ����ͼ����canny���Ӽ���Ե�ĳ���"
            "\nʹ��OpenCV version " << CV_VERSION << endl;
    cout << "\nʹ��:\n"
            "\t./canny [ͼƬ���� -- Ĭ��ֵΪ ubuntu.png]\n\n";
	
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
  
    //--------------------------------�� CannyThreshold()���� ��----------------------------------------------
    //         opencv���õ�canny����,������ֵ����
    //-------------------------------------------------------------------------------------------------
    namedWindow( window_name, WINDOW_AUTOSIZE );
    createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
    createTrackbar( "ratio:", window_name, &ratio, max_ratio, CannyThreshold );
    CannyThreshold(0, 0);
    
    //--------------------------------�� myCanny()���� ��----------------------------------------------
    //         �Լ���д��canny����
    //--------------------------------------------------------------------------------------
    Mat dst_myCanny;
    dst_myCanny.create(src.size(), src.type());
    myCanny(src_gray, dst_myCanny, 13, 69, 3); 

    cvNamedWindow("GaussImage",0);  
    cvShowImage("GaussImage",pCanny);               //��ʾ��˹ͼ
    cvWaitKey(0);  
    cvDestroyWindow("GaussImage"); 
    
    
    
    //namedWindow("dst_myCanny", WINDOW_AUTOSIZE);
    //imshow("dst_myCanny", dst_myCanny);

    
    namedWindow("src", WINDOW_AUTOSIZE);
    imshow("src", src);
    
    waitKey(0);
    return 0;
       
}
