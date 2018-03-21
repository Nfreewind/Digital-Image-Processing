/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 模板卷积.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *			
 * Reference               : http://blog.csdn.net/u011503970/article/details/18568043
 * 				http://blog.csdn.net/keith_bb/article/details/53085379
 * 				http://blog.csdn.net/keith_bb/article/details/53103026
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.21, William Yu
                             ver1.1, 2018.03.21, William Yu, add notes
=====================================================================================*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


//只适用于3阶模板
void convolution(const Mat& myImage, Mat& Result, Mat&kernel )
{
    CV_Assert(myImage.depth() == CV_8U);        //判断函数CV_Assert
    const int nChannels = myImage.channels();

    for(int j = 1; j < myImage.rows - 1; ++j)
    {
        const uchar* previous = myImage.ptr<uchar>(j - 1);      //当前像素上一行指针
        const uchar* current = myImage.ptr<uchar>(j);           //当前像素行指针
        const uchar* next = myImage.ptr<uchar>(j + 1);          //当前像素下一行指针

        uchar* output = Result.ptr<uchar>(j);

        //利用公式和上下左右四个像素对当前像素值进行处理
        for(int i = nChannels; i < nChannels * (myImage.cols - 1); ++i)
        {/*
	              *output++ = saturate_cast<uchar>(5*current[i]  
                         -current[i-nChannels] - current[i+nChannels] - previous[i] - next[i]);         // 计算新值  
		    */
            *output++ = saturate_cast<uchar>
            (  kernel.at<int>(0,0)* previous[i-nChannels] +  kernel.at<int>(0,1)* previous[i] +  kernel.at<int>(0,2)* previous[i+nChannels] + 
	       kernel.at<int>(1,0)* current[i-nChannels] +  kernel.at<int>(1,1)* current[i] +  kernel.at<int>(1,2)* current[i+nChannels] + 
	       kernel.at<int>(2,0)* next[i-nChannels] +  kernel.at<int>(2,1)* next[i] +  kernel.at<int>(2,2)* next[i+nChannels]  );
        }
    }
    Result.row(0).setTo(Scalar(0));                 //设置第一行所有元素值为0
    Result.row(Result.rows-1).setTo(Scalar(0));     //设置最后一行所有元素值为0
    Result.col(0).setTo(Scalar(0));                 //设置第一列所有元素值为0
    Result.col(Result.cols-1).setTo(Scalar(0));     //设置最后一列所有元素值为0
}

//--------------------------------【 gray_transformation()函数 】----------------------------------------------
//		图像灰度变换
//-------------------------------------------------------------------------------------------------


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

    Mat srcImage;
    srcImage= imread(filename, 1);
    if(srcImage.empty()) {printf("Cannot read image file: %s\n", filename.c_str()); return -1; }
    
    Mat dstImageMyCon;
    dstImageMyCon.create(srcImage.size(), srcImage.type());
    
    Mat dstImageOpencv;
    dstImageOpencv.create(srcImage.size(), srcImage.type());
    
    Mat kernel = (Mat_<int>(3,3) << 0, -1 ,0,
                                    -1, 5, -1,
                                     0, -1, 0);
     
    //--------------------------------【 convolution()函数 】----------------------------------------------
    //         图像灰度变换参数说明：
    //              void convolution(const Mat& myImage, Mat& myImage)
    //				参数 myImage: 传入原图
    //				参数 myImage: 传出待处理图像
    //				参数
    //-------------------------------------------------------------------------------------------------
    convolution(srcImage, dstImageMyCon, kernel);  
    //保证安全,归一化到0~255  
    normalize(dstImageMyCon, dstImageMyCon, 0, 255, CV_MINMAX);

    //--------------------------------【 filter2D()函数 】----------------------------------------------
    //         opencv内置的卷积操作函数
    //-------------------------------------------------------------------------------------------------
    filter2D(srcImage, dstImageOpencv, srcImage.depth(), kernel);
    
    
    
    namedWindow("srcImage", WINDOW_AUTOSIZE);
    imshow("srcImage", srcImage);
    namedWindow("dstImageMyCon",WINDOW_AUTOSIZE);
    imshow("dstImageMyCon",dstImageMyCon);
    namedWindow("dstImageOpencv", WINDOW_AUTOSIZE);
    imshow("dstImageOpencv", dstImageOpencv);
    waitKey(0);
    return 0;
}
