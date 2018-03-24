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
int const max_lowThreshold = 100;
int ratio = 3;
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
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
}
////////////////////【注意】此处比较有趣的想法,当不能直接处理彩色图片时，先处理其灰度图，再叠加回彩色图片上面去，满分操作///////////


/*
//--------------------------------【 myCanny()函数 】----------------------------------------------
//        参数说明：
//            void Adaptive_Median_Filter(const Mat& src, Mat& dst, int kernal_size)
//			参数 & src: 
//			参数 & dst: 
//			参数 kernal_size:
//-------------------------------------------------------------------------------------------------
void myCanny( InputArray image, OutputArray edges, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false )
{
  
}
*/

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
    CannyThreshold(0, 0);
    
    //--------------------------------【 myCanny()函数 】----------------------------------------------
    //         自己编写的canny算子
    //--------------------------------------------------------------------------------------
    //Mat dst_myCanny;
    //dst_myCanny.create(src.size(), src.type());
    //myCanny(src, dst_myCanny, 13, 69, 3); 
    //namedWindow("dst_myCanny", WINDOW_AUTOSIZE);
    //imshow("dst_myCanny", dst_myCanny);

    
    namedWindow("src", WINDOW_AUTOSIZE);
    imshow("src", src);
    
    waitKey(0);
    return 0;
       
}