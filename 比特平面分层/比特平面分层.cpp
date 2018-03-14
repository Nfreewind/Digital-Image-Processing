/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 比特平面分层.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *			
 * Reference               : http://blog.csdn.net/liuchuan__________/article/details/48520655
 * 		         	http://blog.csdn.net/qq_19332527/article/details/78331240
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.13, William Yu
                             ver1.1, 2018.03.13, William Yu, add notes
=====================================================================================*/
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//--------------------------------【 binary()函数 】----------------------------------------------
//		传入像素灰度值，并移位提取每一位
//-------------------------------------------------------------------------------------------------
int b[8]= { 0 };  
void  binary(int num)  
{  
  int i=0;
  while (num != 0)  
  {  
       b[i] = num % 2;  
       num = num / 2;  
       i++;  
  }  
} 


static void help()
{
    cout << "\n这是一个演示数字图像处理灰度图图像标度处理的程序"
            "\n使用OpenCV version " << CV_VERSION << endl;
    cout << "\n使用:\n"
            "\t./比特平面分层 [图片名称 -- 默认值为 ubuntu.png]\n\n";
	
    cout << "\nThis is a demo of ,"
            "\nUsing OpenCV version " << CV_VERSION << endl;
    cout << "\nCall:\n"
            "    ./比特平面分层 [image_name -- Default is ubuntu.png]\n\n";
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
    srcImage= imread(filename,0);
    if(srcImage.empty()) {printf("Cannot read image file: %s\n", filename.c_str()); return -1; }

    Mat d1, d2, d3, d4, d5, d6, d7, d8;  
    d1.create(srcImage.size(), CV_8UC1);
    d2.create(srcImage.size(), CV_8UC1); 
    d3.create(srcImage.size(), CV_8UC1); 
    d4.create(srcImage.size(), CV_8UC1);  
    d5.create(srcImage.size(), CV_8UC1); 
    d6.create(srcImage.size(), CV_8UC1); 
    d7.create(srcImage.size(), CV_8UC1); 
    d8.create(srcImage.size(), CV_8UC1);  
    
    int rowNumber = srcImage.rows,colNumber=srcImage.cols;  
    
    ///遍历像素点
    for(int i=0;i<rowNumber;i++) 
    {
        for (int j = 0; j < colNumber; j++)  
        {  
	    ///提取像素值
            int num = srcImage.at<uchar>(i, j);  
            ///分割位
	    binary(num);
	    ///创建新图片，提取出来的位的数值为0或1，应该是一位二值图，乘以255均值到0和255,补充至8位灰度图。 
            d1.at<uchar>(i, j) = b[0]*255;
	    d2.at<uchar>(i, j) = b[1]*255;
	    d3.at<uchar>(i, j) = b[2]*255;
	    d4.at<uchar>(i, j) = b[3]*255;
            d5.at<uchar>(i, j) = b[4]*255;
	    d6.at<uchar>(i, j) = b[5]*255;
	    d7.at<uchar>(i, j) = b[6]*255;
	    d8.at<uchar>(i, j) = b[7]*255;
        } 
    }
    
    imshow("原图", srcImage);
    imshow("第一层比特图", d1);
    imshow("第二层比特图", d2);
    imshow("第三层比特图", d3);
    imshow("第四层比特图", d4);
    imshow("第五层比特图", d5);
    imshow("第六层比特图", d6);
    imshow("第七层比特图", d7);
    imshow("第八层比特图", d8);
    /*
    imwrite("1.jpg", d1);
    imwrite("2.jpg", d2);
    imwrite("3.jpg", d3);
    imwrite("4.jpg", d4);  
    imwrite("5.jpg", d5); 
    imwrite("6.jpg", d6); 
    imwrite("7.jpg", d7);
    imwrite("8.jpg", d8);  
    */
    
    waitKey(0);
    return 0;
}
