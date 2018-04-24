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
 * 				https://blog.csdn.net/dcrmg/article/details/52344902
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




//--------------------------------【 myconvolution()函数 】----------------------------------------------
//	void myconvolution(const Mat& myImage, Mat& Result, double ** kernel, int ksize);
//		参数
//			& myImage  :输入图像
//			& Result   :输出图像
//			& kernel   :卷积核
//			ksize      :卷积核大小
//------------------------------------------------------------------------------------------------- 
void myconvolution(const Mat& myImage, Mat& Result, double ** kernel, int ksize)
{
    const int nChannels = myImage.channels();
    int center =(int) ((ksize)/2);
    
    for(int j = center; j < myImage.rows - center; ++j)
    {
	///创建行指针首地址序列
	vector<const uchar*> col_ptrs;
	for(int k= 0; k< ksize; ++k)
	{
	  const uchar* col_ptr = myImage.ptr<uchar>(j- center + k );  
	  col_ptrs.push_back(col_ptr);
	}
	
	uchar* output = Result.ptr<uchar>(j); //结果图片行首地址
	output += nChannels * center; //将行首地址加偏移量
	
        for(int i = nChannels * center; i < nChannels*myImage.cols - nChannels * center; ++i)
        {
	  ///卷积操作
	  double temp = 0;
	  for(int aaa=0; aaa<ksize; ++aaa) 
	  {
	     for(int bbb=0; bbb<ksize; ++bbb)
	     {
		temp += kernel[aaa][bbb] * col_ptrs[aaa][i - nChannels*center + nChannels*bbb];
	     }
	  }
	  ///saturate_cast函数,强制限定为uchar ： a>255则a=255,a<0，则a=0
	  *output++ = saturate_cast<uchar>(temp); 
	}
    }
}

//--------------------------------【 mygetGaussianKernel()函数 】----------------------------------------------
//	double ** mygetGaussianKernel(int ksize, double sigma);
//		参数
//			ksize      :模板大小
//			sigma      :高斯标准差
//------------------------------------------------------------------------------------------------- 
double ** mygetGaussianKernel(int ksize, double sigma)
{
    int i, j;  
    double sum = 0.0;  
    int center = ksize/2;
    
    ///建立一个size*size大小的动态二维数组  
    double **arr = new double*[ksize];
    for (i = 0; i < ksize; ++i)  
        arr[i] = new double[ksize];  
    
    ///高斯函数  
    for (i = 0; i < ksize; ++i)  
        for (j = 0; j < ksize; ++j) 
	{  
            arr[i][j] = exp(-((i - center)*(i - center) + (j - center)*(j - center)) / (sigma*sigma * 2));  
            sum += arr[i][j];  
        }
        
    ///归一化
    double chacksum=0;
    for (i = 0; i < ksize; ++i)  
        for (j = 0; j < ksize; ++j)  
	{   
	  arr[i][j] /= sum;
	  chacksum += arr[i][j];
	}
    /*
    ///输出
    printf("%.10f ",chacksum);
    for (i = 0; i < ksize; ++i)
      for (j = 0; j < ksize; ++j)
      {
	if ( j%ksize ==0) { printf("\n");}
	printf("%.10f ",arr[i][j]);
      }
    */
    return arr;
}


//--------------------------------【 myGaussianBlur()函数 】----------------------------------------------
//	void myGaussianBlur(Mat& srcImage, Mat& dstImage, int ksize, int sigma);
//		描述	
//			高斯平滑滤波
//		参数 
//			& srcImage :输入图像
//			& dstImage :输出图像
//			ksize      :模板大小
//			sigma      :高斯标准差
//-------------------------------------------------------------------------------------------------    
void myGaussianBlur(const Mat& srcImage, Mat& dstImage, int ksize, double sigma)
{
    if(srcImage.empty()) {printf("Cannot read image file"); return; }
    //--------------------------------【 mygetGaussianKernel()函数 】----------------------------------------------
    //		获取高斯滤波器系数. 
    //		Returns Gaussian filter coefficients.
    //-------------------------------------------------------------------------------------------------
    double **kernel; 
    kernel = mygetGaussianKernel(ksize, sigma); 
    
    //--------------------------------【 convolution()函数 】----------------------------------------------
    //		卷积计算
    //-------------------------------------------------------------------------------------------------
    myconvolution(srcImage, dstImage, kernel, ksize);
}

/******************Sobel卷积因子计算X、Y方向梯度和梯度方向角********************  
//第一个参数imageSourc原始灰度图像；  
//第二个参数imageSobelX是X方向梯度图像；  
//第三个参数imageSobelY是Y方向梯度图像；  
//第四个参数pointDrection是梯度方向角数组指针  
//*************************************************************/
void SobelGradDirction(const Mat& imageSource,Mat &imageSobelX,Mat &imageSobelY,double *&pointDrection)  
{  
    pointDrection=new double[(imageSource.rows-1)*(imageSource.cols-1)];  
    for(int i=0;i<(imageSource.rows-1)*(imageSource.cols-1);i++)  
    {  
        pointDrection[i]=0;  
    }  
    imageSobelX=Mat::zeros(imageSource.size(),CV_32SC1);  
    imageSobelY=Mat::zeros(imageSource.size(),CV_32SC1);  
    uchar *P=imageSource.data;    
    uchar *PX=imageSobelX.data;    
    uchar *PY=imageSobelY.data;    
  
    int step=imageSource.step;    
    int stepXY=imageSobelX.step;   
    int k=0;  
    int m=0;  
    int n=0;  
    for(int i=1;i<(imageSource.rows-1);i++)    
    {    
        for(int j=1;j<(imageSource.cols-1);j++)    
        {    
            //通过指针遍历图像上每一个像素   
            double gradY=P[(i-1)*step+j+1]+P[i*step+j+1]*2+P[(i+1)*step+j+1]-P[(i-1)*step+j-1]-P[i*step+j-1]*2-P[(i+1)*step+j-1];  
            PY[i*stepXY+j*(stepXY/step)]=abs(gradY);  
            double gradX=P[(i+1)*step+j-1]+P[(i+1)*step+j]*2+P[(i+1)*step+j+1]-P[(i-1)*step+j-1]-P[(i-1)*step+j]*2-P[(i-1)*step+j+1];  
            PX[i*stepXY+j*(stepXY/step)]=abs(gradX);  
            if(gradX==0)  
            {  
                gradX=0.00000000000000001;  //防止除数为0异常  
            }  
            pointDrection[k]=atan(gradY/gradX)*57.3;//弧度转换为度  
            pointDrection[k]+=90;  
            k++;  
        }    
    }   
    convertScaleAbs(imageSobelX,imageSobelX);  
    convertScaleAbs(imageSobelY,imageSobelY);  
}  

//******************计算Sobel的X和Y方向梯度幅值*************************  
//第一个参数imageGradX是X方向梯度图像；  
//第二个参数imageGradY是Y方向梯度图像；  
//第三个参数SobelAmpXY是输出的X、Y方向梯度图像幅值  
//*************************************************************  
void SobelAmplitude(const Mat imageGradX,const Mat imageGradY,Mat &SobelAmpXY)  
{  
    SobelAmpXY=Mat::zeros(imageGradX.size(),CV_32FC1);  
    for(int i=0;i<SobelAmpXY.rows;i++)  
    {  
        for(int j=0;j<SobelAmpXY.cols;j++)  
        {  
            SobelAmpXY.at<float>(i,j)=sqrt(imageGradX.at<uchar>(i,j)*imageGradX.at<uchar>(i,j)+imageGradY.at<uchar>(i,j)*imageGradY.at<uchar>(i,j));  
        }  
    }  
    convertScaleAbs(SobelAmpXY,SobelAmpXY);  
}  

//******************局部极大值抑制*************************  
//第一个参数imageInput输入的Sobel梯度图像；  
//第二个参数imageOutPut是输出的局部极大值抑制图像；  
//第三个参数pointDrection是图像上每个点的梯度方向数组指针  
//*************************************************************  
void LocalMaxValue(const Mat imageInput,Mat &imageOutput,double *pointDrection)  
{  
    //imageInput.copyTo(imageOutput);  
    imageOutput=imageInput.clone();  
    int k=0;  
    for(int i=1;i<imageInput.rows-1;i++)  
    {  
        for(int j=1;j<imageInput.cols-1;j++)  
        {  
            int value00=imageInput.at<uchar>((i-1),j-1);  
            int value01=imageInput.at<uchar>((i-1),j);  
            int value02=imageInput.at<uchar>((i-1),j+1);  
            int value10=imageInput.at<uchar>((i),j-1);  
            int value11=imageInput.at<uchar>((i),j);  
            int value12=imageInput.at<uchar>((i),j+1);  
            int value20=imageInput.at<uchar>((i+1),j-1);  
            int value21=imageInput.at<uchar>((i+1),j);  
            int value22=imageInput.at<uchar>((i+1),j+1);  
  
            if(pointDrection[k]>0&&pointDrection[k]<=45)  
            {  
                if(value11<=(value12+(value02-value12)*tan(pointDrection[i*imageOutput.rows+j]))||(value11<=(value10+(value20-value10)*tan(pointDrection[i*imageOutput.rows+j]))))  
                {  
                    imageOutput.at<uchar>(i,j)=0;  
                }  
            }     
            if(pointDrection[k]>45&&pointDrection[k]<=90)  
  
            {  
                if(value11<=(value01+(value02-value01)/tan(pointDrection[i*imageOutput.cols+j]))||value11<=(value21+(value20-value21)/tan(pointDrection[i*imageOutput.cols+j])))  
                {  
                    imageOutput.at<uchar>(i,j)=0;  
  
                }  
            }  
            if(pointDrection[k]>90&&pointDrection[k]<=135)  
            {  
                if(value11<=(value01+(value00-value01)/tan(180-pointDrection[i*imageOutput.cols+j]))||value11<=(value21+(value22-value21)/tan(180-pointDrection[i*imageOutput.cols+j])))  
                {  
                    imageOutput.at<uchar>(i,j)=0;  
                }  
            }  
            if(pointDrection[k]>135&&pointDrection[k]<=180)  
            {  
                if(value11<=(value10+(value00-value10)*tan(180-pointDrection[i*imageOutput.cols+j]))||value11<=(value12+(value22-value11)*tan(180-pointDrection[i*imageOutput.cols+j])))  
                {  
                    imageOutput.at<uchar>(i,j)=0;  
                }  
            }  
            k++;  
        }  
    }  
}

//******************双阈值处理*************************  
//第一个参数imageInput输入和输出的的Sobel梯度幅值图像；  
//第二个参数lowThreshold是低阈值  
//第三个参数highThreshold是高阈值  
//******************************************************  
void DoubleThreshold(Mat &imageIput,double lowThreshold,double highThreshold)  
{  
    for(int i=0;i<imageIput.rows;i++)  
    {  
        for(int j=0;j<imageIput.cols;j++)  
        {  
            if(imageIput.at<uchar>(i,j)>highThreshold)  
            {  
                imageIput.at<uchar>(i,j)=255;  
            }     
            if(imageIput.at<uchar>(i,j)<lowThreshold)  
            {  
                imageIput.at<uchar>(i,j)=0;  
            }     
        }  
    }  
}  

//******************双阈值中间像素连接处理*********************  
//第一个参数imageInput输入和输出的的Sobel梯度幅值图像；  
//第二个参数lowThreshold是低阈值  
//第三个参数highThreshold是高阈值  
//*************************************************************  
void DoubleThresholdLink(Mat &imageInput,double lowThreshold,double highThreshold)  
{  
    for(int i=1;i<imageInput.rows-1;i++)  
    {  
        for(int j=1;j<imageInput.cols-1;j++)  
        {  
            if(imageInput.at<uchar>(i,j)>lowThreshold&&imageInput.at<uchar>(i,j)<255)  
            {  
                if(imageInput.at<uchar>(i-1,j-1)==255||imageInput.at<uchar>(i-1,j)==255||imageInput.at<uchar>(i-1,j+1)==255||  
                    imageInput.at<uchar>(i,j-1)==255||imageInput.at<uchar>(i,j)==255||imageInput.at<uchar>(i,j+1)==255||  
                    imageInput.at<uchar>(i+1,j-1)==255||imageInput.at<uchar>(i+1,j)==255||imageInput.at<uchar>(i+1,j+1)==255)  
                {  
                    imageInput.at<uchar>(i,j)=255;  
                    DoubleThresholdLink(imageInput,lowThreshold,highThreshold); //递归调用  
                }     
                else  
            {  
                    imageInput.at<uchar>(i,j)=0;  
            }                 
            }                 
        }  
    }  
}  


//--------------------------------【 myCanny()函数 】----------------------------------------------
//	Canny算子求边缘点具体算法步骤如下：
//			1. 用高斯滤波器平滑图像
//			2. 用一阶偏导有限差分计算梯度幅值和方向
//			3. 对梯度幅值进行非极大值抑制
//			4. 用双阈值算法检测和连接边缘        
//	void myCanny()
//		参数 : 
//		
//-------------------------------------------------------------------------------------------------
void myCanny(Mat& src, Mat& dst, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false )
{
    //-----------------------------------------------------------------------------------
    //	高斯平滑滤波
    //---------------------------------------------------------------
    Mat dst_GaussianBlur;
    dst_GaussianBlur.create(src.size(), src.type());
    int ksize= 5;   
    double sigma = 0.3*((ksize-1)*0.5 - 1) + 0.8;
    myGaussianBlur(src, dst_GaussianBlur, ksize, sigma);
    namedWindow("GaussBlur",1);  
    imshow("GaussBlur",dst_GaussianBlur);
    waitKey(0);  
    destroyWindow("GaussBlur"); 
    
    
    //-----------------------------------------------------------------------------------
    //	用Sobel等梯度算子计算梯度幅值和方向
    //---------------------------------------------------------------
    //可以用不同的检测器
    //P[i,j]=(S[i+1,j]-S[i,j]+S[i+1,j+1]-S[i,j+1])/2
    //Q[i,j]=(S[i,j]-S[i,j+1]+S[i+1,j]-S[i+1,j+1])/2
    //---------------------------------------------------------------
    Mat imageSobelX;
    imageSobelX.create(src.size(), src.type());
    Mat imageSobelY;
    imageSobelY.create(src.size(), src.type());
    double *pointDrection=new double[src.cols*src.rows];
    SobelGradDirction(dst_GaussianBlur, imageSobelX, imageSobelY, pointDrection);
    imshow("imageSobelX",imageSobelX);
    imshow("imageSobelY",imageSobelY);
    waitKey(0);  
    destroyWindow("imageSobelX");
    destroyWindow("imageSobelY"); 
   
    Mat SobelAmpXY;
    SobelAmpXY.create(src.size(), src.type());
    SobelAmplitude(imageSobelX, imageSobelY, SobelAmpXY);  
    imshow("SobelAmpXY",SobelAmpXY);
    waitKey(0);  
    destroyWindow("SobelAmpXY");
    
    Mat limit;
    limit.create(src.size(), src.type());
    LocalMaxValue( imageSobelX, limit, pointDrection); 
    imshow("limit",limit);
    waitKey(0);  
    destroyWindow("limit");
    
    DoubleThreshold(limit,60,100);
    imshow("DoubleThreshold",limit);
    waitKey(0);  
    destroyWindow("DoubleThreshold");
    
    DoubleThresholdLink(limit, 60, 100);
    imshow("Canny Image",limit);
    waitKey(0);  
    destroyWindow("Canny Image");
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
    //help();
    CommandLineParser parser(argc, argv, keys); 
    if (parser.has("help"))  { help();  return 0; }
    string filename = parser.get<string>(0);
    //Mat src;
    src= imread(filename, 1);
    if(src.empty()) {printf("Cannot read image file: %s\n", filename.c_str()); return -1; }
    namedWindow("src", WINDOW_AUTOSIZE);
    imshow("src", src);
    
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
    
    //namedWindow("dst_myCanny", WINDOW_AUTOSIZE);
    //imshow("dst_myCanny", dst_myCanny);
    
    waitKey(0);
    return 0;
       
}
