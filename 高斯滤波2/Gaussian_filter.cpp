/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : Gaussian_filter.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *			
 * Reference               : http://www.cnblogs.com/love6tao/p/5152020.html
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.22, William Yu
                             ver1.1, 2018.03.22, William Yu, add notes
=====================================================================================*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//--------------------------------【 myGaussianBlur()函数 】----------------------------------------------
//        参数说明：
//            void myGaussianBlur(Mat& src_gray, Mat& dst, int ksize, int sigma)
//			参数 & src_gray: 
//			参数 & dst: 
//			参数 ksize:
//			参数 sigma:
//-------------------------------------------------------------------------------------------------    
void myGaussianBlur(const Mat src, const Mat& src_gray, Mat& dst, int ksize, int sigma)
{
    double nSigma = 0.2;
    int nWindowSize = 1 + 2*ceil(3*nSigma);//通过sigma得到窗口大小
    int nCenter = nWindowSize/2;
    int nWidth = src_gray.cols;
    int nHeight = src_gray.rows;
    IplImage * pDst;
    pDst = cvCreateImage( src.size(), src.depth(), 1);
    //生成二维滤波核
    double *pKernel_2 = new double[nWindowSize*nWindowSize];
    double d_sum = 0.0;
    for(int i=0;i<nWindowSize;i++)
    {
        for (int j=0;j<nWindowSize;j++)
        {
            double n_Disx=i-nCenter;//水平方向距离中心像素距离
            double n_Disy=j-nCenter;
            pKernel_2[j*nWindowSize+i]=exp(-0.5*(n_Disx*n_Disx+n_Disy*n_Disy)/(nSigma*nSigma))/(2.0*3.1415926)*nSigma*nSigma; 
            d_sum=d_sum+pKernel_2[j*nWindowSize+i];
        }
    }
    for(int i=0;i<nWindowSize;i++)//归一化处理
    {
        for (int j=0;j<nWindowSize;j++)
        {
          pKernel_2[j*nWindowSize+i]=pKernel_2[j*nWindowSize+i]/d_sum;
        }
    }
    //输出模板
    for (int i=0;i<nWindowSize*nWindowSize;i++)
    {
        if (i%(nWindowSize)==0)
        {
          printf("\n");
        }
        printf("%.10f ",pKernel_2[i]);
    }
    //滤波处理
    for (int s=0;s<nWidth;s++)
    {
        for (int t=0;t<nHeight;t++)
        {
            double dFilter=0;
            double dSum=0;
            //当前坐标（s,t）
            //获取8邻域
            for (int x=-nCenter;x<=nCenter;x++)
            {
                for (int y=-nCenter;y<=nCenter;y++)
                {
                    if ( (x+s>=0) && (x+s<nWidth) && (y+t>=0) && (y+t<nHeight) )//判断是否越界
                    {
                        double currentvalue = (double) src_gray.data[(y+t)* src_gray.cols +x+s];
                        dFilter += currentvalue* pKernel_2 [x+nCenter+(y+nCenter)*nCenter];
                        dSum += pKernel_2 [x+nCenter+(y+nCenter)*nCenter];
                    }
                }
            }
            pDst -> imageData [t* pDst -> widthStep + s]=(uchar) (dFilter/dSum);
        }
    }
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
    
    Mat src_gray;
    cvtColor( srcImage, src_gray, COLOR_BGR2GRAY );
     
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
    myGaussianBlur(srcImage, src_gray, dst_myGaussianBlur, ksize, sigma);
    //保证安全,归一化到0~255  
    normalize(dst_myGaussianBlur, dst_myGaussianBlur, 0, 255, CV_MINMAX);
    cvNamedWindow("GaussImage",0);  

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
