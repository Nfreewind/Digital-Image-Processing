/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : equalizeHist.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *			      对一个灰度图做直方图均衡化    
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.12, William Yu
                             ver1.1, 2018.03.12, William Yu, add notes
=====================================================================================*/
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;


static void help()
{
    cout << "\n这是一个演示数字图像处理灰度图直方图均值化的程序"
            "\n使用OpenCV version " << CV_VERSION << endl;
    cout << "\n使用：\n"
            "\t./equalizeHist [图片名称 -- 默认值为ubuntu.png]\n\n";

    cout << "\nThis is a demo of scale a gray image,"
            "\nUsing OpenCV version " << CV_VERSION << endl;
    cout << "\nCall:\n"
            "    ./scale [image_name -- Default is ubuntu.png]\n\n";
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

    Mat srcImage, dstImage;
    srcImage= imread(filename, 0);
    if(srcImage.empty()) {printf("Cannot read image file: %s\n", filename.c_str()); return -1; }
 
    //--------------------------------【 equalizeHist() 】----------------------------------------------
    //      opencv提供的直方图均衡化	  
    //-----------------------------------------------------------------------------------------
    equalizeHist(srcImage, dstImage);
    
    namedWindow("灰度图");
    namedWindow("均衡化");
    imshow("灰度图",srcImage);
    imshow("均衡化",dstImage);
    
    waitKey(0);
    return 0;
}
