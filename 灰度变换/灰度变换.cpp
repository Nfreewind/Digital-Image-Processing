/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 灰度变换.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *			
 * Reference               : http://www.cnblogs.com/wangguchangqing/p/6983680.html
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.14, William Yu
                             ver1.1, 2018.03.14, William Yu, add notes
=====================================================================================*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

  
//8位图
#define L 255
float gam = 1;
//图像反转
float imgnega(float r,int c){ return L-1-r; }
//对数变换
float imglog(float r,int c){ return c*log(1+r); }
//幂次变化
float imggama(float r,int c){ float temp= pow(r,gam); return c*temp; }



//--------------------------------【 gray_transformation()函数 】----------------------------------------------
//         图像灰度变换参数说明：
//              void gray_transformation(Mat& inputImage, Mat& outputImage, int Eh_Type, int c)
//				参数Eh_Type: 1:图像反转  2：对数变化   3：伽马变换
//				参数c:常数
//-------------------------------------------------------------------------------------------------

void gray_transformation(Mat& inputImage, Mat& outputImage, int Eh_Type,int c)
{
	outputImage = inputImage.clone();
	int rowNumber = outputImage.rows;
	int colNumber = outputImage.cols;

	//函数指针
	typedef float (*FP)(float, int);
	FP func_list[]={ imgnega , imglog , imggama };
	
	for(int i = 0;i < rowNumber;i++)  //行循环
	{
		for(int j = 0; j < colNumber; j++)   //列循环
		{
		        ///提取像素值
			float num = outputImage.at<uchar>(i, j);  
			// ---------【开始处理每个像素】-------------
			outputImage.at<uchar>(i, j) = func_list[Eh_Type-1](num, c); 
			//此处func_list[Eh_Type-1](num,c); 也可以写成(*func_list[Eh_Type-1])(num,c)
			// ----------【处理结束】---------------------
		}
	}
}


static void help()
{
    cout << "\n这是一个演示数字图像处理灰度图图像灰度变换的程序"
            "\n使用OpenCV version " << CV_VERSION << endl;
    cout << "\n使用:\n"
            "\t./灰度变换 [图片名称 -- 默认值为 ubuntu.png]\n\n";
	
    cout << "\nThis is a demo of ,"
            "\nUsing OpenCV version " << CV_VERSION << endl;
    cout << "\nCall:\n"
            "    ./灰度变换 [image_name -- Default is ubuntu.png]\n\n";
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

    Mat srcImage, rstImage1, rstImage2, rstImage3;
    srcImage= imread(filename,0);
    if(srcImage.empty()) {printf("Cannot read image file: %s\n", filename.c_str()); return -1; }
    
//--------------------------------【 gray_transformation()函数 】----------------------------------------------
//		图像灰度变换
//-------------------------------------------------------------------------------------------------
    do{ 
      cout << "s=c*r^gam，输入指数gam，注意范围gam>0：\n";
      cin >> gam;
    } while (gam < 0);
    
    gray_transformation(srcImage, rstImage1, 1, 1);
    gray_transformation(srcImage, rstImage2, 2, 1);
    gray_transformation(srcImage, rstImage3, 3, 1);

    //归一化到0~255  
    normalize(rstImage1, rstImage1, 0, 255, CV_MINMAX);
    normalize(rstImage2, rstImage2, 0, 255, CV_MINMAX);
    normalize(rstImage3, rstImage3, 0, 255, CV_MINMAX);
    
    imshow("原灰度图", srcImage);
    //归一化原灰度图作对比
    normalize(srcImage, srcImage, 0, 255, CV_MINMAX);
    imshow("原灰度图归一化", srcImage);
    imshow("反转变换", rstImage1);
    imshow("对数变换", rstImage2);
    imshow("伽马变换", rstImage3);

    waitKey(0);
    return 0;
}
