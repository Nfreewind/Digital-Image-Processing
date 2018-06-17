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




//--------------------------------�� myconvolution()���� ��----------------------------------------------
//	void myconvolution(const Mat& myImage, Mat& Result, double ** kernel, int ksize);
//		����
//			& myImage  :����ͼ��
//			& Result   :���ͼ��
//			& kernel   :�����
//			ksize      :����˴�С
//------------------------------------------------------------------------------------------------- 
void myconvolution(const Mat& myImage, Mat& Result, double ** kernel, int ksize)
{
    const int nChannels = myImage.channels();
    int center =(int) ((ksize)/2);
    
    for(int j = center; j < myImage.rows - center; ++j)
    {
	///������ָ���׵�ַ����
	vector<const uchar*> col_ptrs;
	for(int k= 0; k< ksize; ++k)
	{
	  const uchar* col_ptr = myImage.ptr<uchar>(j- center + k );  
	  col_ptrs.push_back(col_ptr);
	}
	
	uchar* output = Result.ptr<uchar>(j); //���ͼƬ���׵�ַ
	output += nChannels * center; //�����׵�ַ��ƫ����
	
        for(int i = nChannels * center; i < nChannels*myImage.cols - nChannels * center; ++i)
        {
	  ///�������
	  double temp = 0;
	  for(int aaa=0; aaa<ksize; ++aaa) 
	  {
	     for(int bbb=0; bbb<ksize; ++bbb)
	     {
		temp += kernel[aaa][bbb] * col_ptrs[aaa][i - nChannels*center + nChannels*bbb];
	     }
	  }
	  ///saturate_cast����,ǿ���޶�Ϊuchar �� a>255��a=255,a<0����a=0
	  *output++ = saturate_cast<uchar>(temp); 
	}
    }
}

//--------------------------------�� mygetGaussianKernel()���� ��----------------------------------------------
//	double ** mygetGaussianKernel(int ksize, double sigma);
//		����
//			ksize      :ģ���С
//			sigma      :��˹��׼��
//------------------------------------------------------------------------------------------------- 
double ** mygetGaussianKernel(int ksize, double sigma)
{
    int i, j;  
    double sum = 0.0;  
    int center = ksize/2;
    
    ///����һ��size*size��С�Ķ�̬��ά����  
    double **arr = new double*[ksize];
    for (i = 0; i < ksize; ++i)  
        arr[i] = new double[ksize];  
    
    ///��˹����  
    for (i = 0; i < ksize; ++i)  
        for (j = 0; j < ksize; ++j) 
	{  
            arr[i][j] = exp(-((i - center)*(i - center) + (j - center)*(j - center)) / (sigma*sigma * 2));  
            sum += arr[i][j];  
        }
        
    ///��һ��
    double chacksum=0;
    for (i = 0; i < ksize; ++i)  
        for (j = 0; j < ksize; ++j)  
	{   
	  arr[i][j] /= sum;
	  chacksum += arr[i][j];
	}
    /*
    ///���
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


//--------------------------------�� myGaussianBlur()���� ��----------------------------------------------
//	void myGaussianBlur(Mat& srcImage, Mat& dstImage, int ksize, int sigma);
//		����	
//			��˹ƽ���˲�
//		���� 
//			& srcImage :����ͼ��
//			& dstImage :���ͼ��
//			ksize      :ģ���С
//			sigma      :��˹��׼��
//-------------------------------------------------------------------------------------------------    
void myGaussianBlur(const Mat& srcImage, Mat& dstImage, int ksize, double sigma)
{
    if(srcImage.empty()) {printf("Cannot read image file"); return; }
    //--------------------------------�� mygetGaussianKernel()���� ��----------------------------------------------
    //		��ȡ��˹�˲���ϵ��. 
    //		Returns Gaussian filter coefficients.
    //-------------------------------------------------------------------------------------------------
    double **kernel; 
    kernel = mygetGaussianKernel(ksize, sigma); 
    
    //--------------------------------�� convolution()���� ��----------------------------------------------
    //		�������
    //-------------------------------------------------------------------------------------------------
    myconvolution(srcImage, dstImage, kernel, ksize);
}

/******************Sobel������Ӽ���X��Y�����ݶȺ��ݶȷ����********************  
//��һ������imageSourcԭʼ�Ҷ�ͼ��  
//�ڶ�������imageSobelX��X�����ݶ�ͼ��  
//����������imageSobelY��Y�����ݶ�ͼ��  
//���ĸ�����pointDrection���ݶȷ��������ָ��  
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
            //ͨ��ָ�����ͼ����ÿһ������   
            double gradY=P[(i-1)*step+j+1]+P[i*step+j+1]*2+P[(i+1)*step+j+1]-P[(i-1)*step+j-1]-P[i*step+j-1]*2-P[(i+1)*step+j-1];  
            PY[i*stepXY+j*(stepXY/step)]=abs(gradY);  
            double gradX=P[(i+1)*step+j-1]+P[(i+1)*step+j]*2+P[(i+1)*step+j+1]-P[(i-1)*step+j-1]-P[(i-1)*step+j]*2-P[(i-1)*step+j+1];  
            PX[i*stepXY+j*(stepXY/step)]=abs(gradX);  
            if(gradX==0)  
            {  
                gradX=0.00000000000000001;  //��ֹ����Ϊ0�쳣  
            }  
            pointDrection[k]=atan(gradY/gradX)*57.3;//����ת��Ϊ��  
            pointDrection[k]+=90;  
            k++;  
        }    
    }   
    convertScaleAbs(imageSobelX,imageSobelX);  
    convertScaleAbs(imageSobelY,imageSobelY);  
}  

//******************����Sobel��X��Y�����ݶȷ�ֵ*************************  
//��һ������imageGradX��X�����ݶ�ͼ��  
//�ڶ�������imageGradY��Y�����ݶ�ͼ��  
//����������SobelAmpXY�������X��Y�����ݶ�ͼ���ֵ  
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

//******************�ֲ�����ֵ����*************************  
//��һ������imageInput�����Sobel�ݶ�ͼ��  
//�ڶ�������imageOutPut������ľֲ�����ֵ����ͼ��  
//����������pointDrection��ͼ����ÿ������ݶȷ�������ָ��  
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

//******************˫��ֵ����*************************  
//��һ������imageInput���������ĵ�Sobel�ݶȷ�ֵͼ��  
//�ڶ�������lowThreshold�ǵ���ֵ  
//����������highThreshold�Ǹ���ֵ  
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

//******************˫��ֵ�м��������Ӵ���*********************  
//��һ������imageInput���������ĵ�Sobel�ݶȷ�ֵͼ��  
//�ڶ�������lowThreshold�ǵ���ֵ  
//����������highThreshold�Ǹ���ֵ  
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
                    DoubleThresholdLink(imageInput,lowThreshold,highThreshold); //�ݹ����  
                }     
                else  
            {  
                    imageInput.at<uchar>(i,j)=0;  
            }                 
            }                 
        }  
    }  
}  


//--------------------------------�� myCanny()���� ��----------------------------------------------
//	Canny�������Ե������㷨�������£�
//			1. �ø�˹�˲���ƽ��ͼ��
//			2. ��һ��ƫ�����޲�ּ����ݶȷ�ֵ�ͷ���
//			3. ���ݶȷ�ֵ���зǼ���ֵ����
//			4. ��˫��ֵ�㷨�������ӱ�Ե        
//	void myCanny()
//		���� : 
//		
//-------------------------------------------------------------------------------------------------
void myCanny(Mat& src, Mat& dst, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false )
{
    //-----------------------------------------------------------------------------------
    //	1.��˹ƽ���˲�
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
    //	2.��Sobel���ݶ����Ӽ����ݶȷ�ֵ�ͷ���
    //---------------------------------------------------------------
    //�����ò�ͬ�ļ����
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
    
    //-----------------------------------------------------------------------------------
    //	3.�Ǽ���ֵ����
    //---------------------------------------------------------------
    Mat limit;
    limit.create(src.size(), src.type());
    LocalMaxValue( imageSobelX, limit, pointDrection);
    imshow("limit",limit);
    waitKey(0);
    destroyWindow("limit");
    
    //-----------------------------------------------------------------------------------
    //	4.˫��ֵ�Լ�˫��ֵ����
    //---------------------------------------------------------------
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
    
    namedWindow("dst_myCanny", WINDOW_AUTOSIZE);
    imshow("dst_myCanny", dst_myCanny);
    
    waitKey(0);
    return 0;
       
}
