/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : scale.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *				对一个灰度图做标度，均衡像素值在0-255之间    
 * Reference               : http://blog.csdn.net/danieljianfeng/article/details/41807809
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.07, William Yu
                             ver1.1, 2018.03.07, William Yu, add notes
=====================================================================================*/
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//--------------------------------【help( )函数】----------------------------------------------
//		描述：输出帮助信息
//-------------------------------------------------------------------------------------------------
static void help()
{
    cout << "\n这是一个演示数字图像处理灰度图图像标度处理的程序"
            "\n使用OpenCV version " << CV_VERSION << endl;
	    
    // print a help message, and the OpenCV version
    cout << "\nThis is a demo of scale a gray image,"
            "\nUsing OpenCV version " << CV_VERSION << endl;
    cout << "\nCall:\n"
            "    ./scale [image_name -- Default is ubuntu.png]\n\n";
}

//--------------------------------【 scale( )函数 】----------------------------------------------
//		描述：遍历像素，寻找最大最小值，并均衡每一个像素值至0-255
//-------------------------------------------------------------------------------------------------
void scale(Mat& inputImage, Mat& outputImage)  
{  
	//参数准备
	outputImage = inputImage.clone();  //拷贝实参到临时变量
	int rowNumber = outputImage.rows;  //行数
	int colNumber = outputImage.cols*outputImage.channels();  //列数 x 通道数=每一行元素的个数
	
	int min;
        int max;
	uchar* data = outputImage.ptr<uchar>(0);  //获取第0行的首地址
	min=data[0];
	max=data[0];
	
	//双重循环，遍历所有的像素值，查找最大以及最小值
	for(int i = 0;i < rowNumber;i++)  //行循环
	{  
		uchar* data = outputImage.ptr<uchar>(i);  //获取第i行的首地址
		for(int j = 0;j < colNumber;j++)   //列循环
		{  	
		    // ---------【开始处理每个像素】-------------  
		    if (min > data[j] )
		      min= data[j];
		    else if (max < data[j] )
		      max= data[j];
		    // ----------【处理结束】---------------------
		}  //行处理结束
	}  
	cout<<endl<<"min:"<<min<<endl;
        cout<<"max:"<<max<<endl;
	
	//双重循环，遍历所有的像素值，进行标度
	for(int i = 0;i < rowNumber;i++)  //行循环
	{  
		uchar* data = outputImage.ptr<uchar>(i);  //获取第i行的首地址
		for(int j = 0;j < colNumber;j++)   //列循环
		{
		    // ---------【开始处理每个像素】-------------  
		    data[j] = ((data[j]-min)*255/(max-min));
		    // ----------【处理结束】---------------------
		}  //行处理结束
	}  
		
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

    Mat src;
    src= imread(filename,0);
    if(src.empty()) {printf("Cannot read image file: %s\n", filename.c_str()); return -1; }
    Mat rst;
    
    //图像标度
    scale(src,rst);

    
    namedWindow("灰度图");
    namedWindow("均衡化");
    imshow("灰度图",src);
    imshow("均衡化",rst);
    
    //为了验证结果，使用opencv提供的函数归定化到0~255  
    normalize(src, src, 0, 255, CV_MINMAX);
    imshow("opencv归定化",rst);
    
    waitKey(0);
    return 0;
}
