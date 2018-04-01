/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : Fourier_transform.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *			
 * Reference               : https://docs.opencv.org/3.3.1/dd/d02/tutorial_js_fourier_transform.html
 *				https://docs.opencv.org/3.3.1/d2/de8/group__core__array.html#gadd6cf9baf2b8b704a11b5f04aaf4f39d
 * 				http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.27, William Yu
                             ver1.1, 2018.03.27, William Yu, add notes
=====================================================================================*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


static void help()
{
    cout << "\n这是一个演示数字图像处理傅里叶变换的程序"
            "\n使用OpenCV version " << CV_VERSION << endl;
    cout << "\n使用:\n"
            "\t./Fourier_transform [图片名称 -- 默认值为 ubuntu.png]\n\n";
	
    cout << "\nThis is a demo of ,"
            "\nUsing OpenCV version " << CV_VERSION << endl;
    cout << "\nCall:\n"
            "    ./Fourier_transform [image_name -- Default is ubuntu.png]\n\n";
}

const char* keys =
{
    "{help h||}{@image |./ubuntu.png |input image name}"
};


int main(int argc, const char ** argv)
{
    help();
    CommandLineParser parser(argc, argv, keys); 
    if (parser.has("help"))  { help();  return 0; }
    string filename = parser.get<string>(0);

    Mat srcImage;
    srcImage= imread(filename, 0);
    if(srcImage.empty()) {printf("Cannot read image file: %s\n", filename.c_str()); return -1; }
    
    Mat dst_opencvFourier_transform;
    //dst_opencvFourier_transform.create(srcImage.size(), srcImage.type());

    //--------------------------------【 离散傅里叶变换 】----------------------------------------------
    //         离散傅里叶变换，将一张图片转换为频率域
    //-------------------------------------------------------------------------------------------------
    
    ///[1]调整图片大小
    //getOptimalDFTSize 返回最理想的适合于傅里叶变换的图像大小
    //see: https://docs.opencv.org/3.3.1/d2/de8/group__core__array.html#ga6577a2e59968936ae02eb2edde5de299
    int M = getOptimalDFTSize( srcImage.rows );
    int N = getOptimalDFTSize( srcImage.cols );
    Mat padded;
    copyMakeBorder(srcImage, padded, 0, M - srcImage.rows, 0, N - srcImage.cols, BORDER_CONSTANT, Scalar::all(0));

    ///【2】
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg);
    
    ///[3]傅里叶变换
    dft(complexImg, complexImg);
    
    ///[4]对数变换
    // compute log(1 + sqrt(Re(DFT(srcImage))**2 + Im(DFT(srcImage))**2))
    split(complexImg, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat mag = planes[0];
    mag += Scalar::all(1);
    log(mag, mag);
    
    ///[5]对角互换
    mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
    int cx = mag.cols/2;
    int cy = mag.rows/2;
    Mat tmp;
    Mat q0(mag, Rect(0, 0, cx, cy));
    Mat q1(mag, Rect(cx, 0, cx, cy));
    Mat q2(mag, Rect(0, cy, cx, cy));
    Mat q3(mag, Rect(cx, cy, cx, cy));
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2); 
    
    ///[6]标定到正常数值
    normalize(mag, dst_opencvFourier_transform, 0, 1, NORM_MINMAX);
    
    
    namedWindow("srcImage", WINDOW_AUTOSIZE);
    imshow("srcImage", srcImage);
    namedWindow("dst_opencvFourier_transform", WINDOW_AUTOSIZE);
    imshow("dst_opencvFourier_transform", dst_opencvFourier_transform);
    waitKey(0);
    return 0;
}
