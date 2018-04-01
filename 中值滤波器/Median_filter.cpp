/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : Median_filter.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *			
 * Reference               : https://docs.opencv.org/3.3.1/d4/d86/group__imgproc__filter.html#ga564869aa33e58769b4469101aac458f9
 *				http://blog.csdn.net/brookicv/article/details/54931857
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.22, William Yu
                             ver1.1, 2018.03.23, William Yu, add notes
=====================================================================================*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//--------------------------------【 adaptiveProcess()函数 】----------------------------------------------
//      void adaptiveProcess(const Mat& src, Mat& dst, int kernelSize, int maxSize)
//		参数 
//			& src       : 
//			& dst       : 
//			kernelSize  :
//			maxSize     :
//-------------------------------------------------------------------------------------------------

uchar one_adaptiveProcess(const Mat& src, int row, int col, int kernelSize, int maxSize)
{
    vector<uchar> pixels;
    for (int a = -kernelSize / 2; a <= kernelSize / 2; a++)
        for (int b = -kernelSize / 2; b <= kernelSize / 2; b++)
        {
            pixels.push_back(src.at<uchar>(row + a, col + b));
        }
    sort(pixels.begin(), pixels.end());
    uchar min = pixels[0];
    uchar max = pixels[kernelSize * kernelSize - 1];
    uchar med = pixels[kernelSize * kernelSize / 2];
    uchar zxy = src.at<uchar>(row, col);
    if (med > min && med < max) // to B
    {
        if (zxy > min && zxy < max)
            return zxy;
        else
            return med;
    }
    else 
    {
        kernelSize += 2;
        if (kernelSize <= maxSize)  // to A  // 增大窗口尺寸，迭代A过程。
            return one_adaptiveProcess(src, row, col, kernelSize, maxSize); 
        else
            return med;
    }
}

void adaptiveProcess(const Mat& src, Mat& dst, int kernelSize, int maxSize)
{
  if(src.empty()) {printf("Cannot read image file"); return; }
  for (int j = maxSize / 2; j < src.rows - maxSize / 2; j++)
  {
    for (int i = maxSize / 2; i < src.cols * src.channels() - maxSize / 2; i++)
    {
      dst.at<uchar>(j, i) = one_adaptiveProcess(src, j, i, kernelSize, maxSize);
    }
  }
}

//--------------------------------【 Adaptive_Median_Filter()函数 】----------------------------------------------
//        参数说明：
//            void Adaptive_Median_Filter(const Mat& src, Mat& dst, int kernal_size)
//			参数 & src: 
//			参数 & dst: 
//			参数 kernal_size:
//-------------------------------------------------------------------------------------------------
//下面的宏，定义了在矩阵src的第m行、n列，ks*ks覆盖的矩形区域内的像素，并将像素压到矢量v中
//该覆盖区域的左上角坐标为（m，n），宽为ks，高为ks，要求src必须是单通道，数据类型为CV_8UC1
#define CV_ROI_ELEM(src,vector,m,n,ks)  \
{                                      \
    uchar* kn;                         \
    int st0=src.step[0];\
    int st1=src.step[1];\
    for(int k=0;k<(ks);k++)            \
    {                                  \
        for(int s=0;s<(ks);s++)        \
        {                              \
            kn =src.data+(k+m)*st0+(s+n)*st1;   \
            vector.push_back(*kn);              \
        }                                       \
    }                                           \
}

#define CV_MAT_ELEM2(src,dtype,y,x) \
    (dtype*)(src.data+src.step[0]*(y)+src.step[1]*(x)) 
    
void Adaptive_Median_Filter(const Mat& src, Mat& dst, int kernal_size)
{
    if(src.empty()) {printf("Cannot read image file"); return; }
    
    uchar* pdst=dst.data;
    uchar Zmin,Zmax,Zmed,Zxy;
    int step0=src.step[0];
    int step1=src.step[1];
    for(int i=kernal_size/2;i<src.rows-kernal_size/2;i++)
    {
        for(int j=kernal_size/2;j<src.cols-kernal_size/2;j++)
        {
            int ks=3;//kernal_size;
            int count=0;
            Zxy=*CV_MAT_ELEM2(src,uchar,i,j);//Sxy覆盖区域的中心点像素值，即锚点像素值
            vector<uchar> v;//将模板覆盖区域的像素，压入矢量v中
            
            do{
                if(cout==0)
                {//获取模板ks*ks覆盖区域的像素，压入矢量v中
                    CV_ROI_ELEM(src,v,i-ks/2,j-ks/2,ks);
                }
                else
                {
               /****************下面的for循环，将外扩的四个边的像素添加到v中**************/
                    uchar* p=src.data+(i-ks/2)*step0+(j-ks/2)*step1;
                    for(int u=0;u<ks;u++)
                    {
                        v.push_back(*(p+u*step1));//向外扩展的四个边的上边
                        v.push_back(*(p+(ks-1)*step0+u*step1));//向外扩展的四个边的下边
                        if(u!=0&&u!=ks-1)
                        {
                          v.push_back( *(p+u*step0));//向外扩展的四个边的左边
                          v.push_back(*(p+u*step0+(ks-1)*step1));//向外扩展的四个边的右边
                        }
                    }
                }

                //对v的元素排序
                //排序后，Sxy覆盖区域内，最大值为Zmax=v[v.size-1],最小值为Zmin=v[0]
                std::sort(v.begin(),v.end());
                Zmin=v[0],Zmax=v[v.size()-1],Zmed=v[ks*ks/2];
                pdst =CV_MAT_ELEM2(dst,uchar,i,j);
                if(Zmin<Zmed&&Zmed<Zmax)
                {
                    if(Zmin<Zxy&&Zxy<Zmax)
                        {*pdst=Zxy;break;}
                    else
                        {*pdst=Zmed;break;}
                }
                else
                {
                    ks +=2;
                }
                count++;
            }while(ks<=kernal_size);

            *pdst=Zmed;
        }
    } 
    
}


static void help()
{
    cout << "\n这是一个演示数字图像处理自适应中值滤波器,以及opencv中值滤波的程序"
            "\n使用OpenCV version " << CV_VERSION << endl;
    cout << "\n使用:\n"
            "\t./Median_filter [图片名称 -- 默认值为 ubuntu.png]\n\n";
	
    cout << "\nThis is a demo of ,"
            "\nUsing OpenCV version " << CV_VERSION << endl;
    cout << "\nCall:\n"
            "    ./Median_filter [image_name -- Default is ubuntu.png]\n\n";
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
    srcImage= imread(filename, 0);
    if(srcImage.empty()) {printf("Cannot read image file: %s\n", filename.c_str()); return -1; }
    
    Mat dst_Adaptive_Median_Filter;
    dst_Adaptive_Median_Filter.create(srcImage.size(), srcImage.type());
    
    Mat dst_opencv_medianBlur;
    dst_opencv_medianBlur.create(srcImage.size(), srcImage.type());

    int ksize= 5;
    
    //--------------------------------【 Adaptive_Median_Filter()函数 】----------------------------------------------
    //		自适应中值平滑滤波...注意:此函数没有添加通道的处理,只接受灰度图
    //-------------------------------------------------------------------------------------------------
    Adaptive_Median_Filter(srcImage, dst_Adaptive_Median_Filter, ksize);
    
    //--------------------------------【 adaptiveProcess()函数 】----------------------------------------------
    //		另外一个自适应中值平滑滤波,做效果对比
    //-------------------------------------------------------------------------------------------------
    int minSize = 3; // 滤波器窗口的起始边长
    int maxSize = 17; // 滤波器窗口的最大边长
    // 扩展图像的边界,这里解决了一个比较难受的模板卷积四边像素的处理问题,采用边界扩展,然后卷积,卷积的结果切除四周黑边即可
    //see: https://docs.opencv.org/3.3.1/d2/de8/group__core__array.html#ga2ac1049c2c3dd25c2b41bffe17658a36
    Mat dst_MakeBorder;
    copyMakeBorder(srcImage, dst_MakeBorder, maxSize / 2, maxSize / 2, maxSize / 2, maxSize / 2, BORDER_REFLECT);
    
    Mat dst_adaptiveProcess;
    dst_adaptiveProcess.create(dst_MakeBorder.size(), dst_MakeBorder.type());
    // 自适应中值滤波器
    adaptiveProcess(dst_MakeBorder, dst_adaptiveProcess, minSize, maxSize);
    
    //--------------------------------【 medianBlur()函数 】----------------------------------------------
    //         opencv内置的中值平滑滤波函数
    //-------------------------------------------------------------------------------------------------
    medianBlur(srcImage, dst_opencv_medianBlur, ksize);
    
    
    namedWindow("srcImage", WINDOW_AUTOSIZE);
    imshow("srcImage", srcImage);
    namedWindow("dst_Adaptive_Median_Filter",WINDOW_AUTOSIZE);
    imshow("dst_Adaptive_Median_Filter",dst_Adaptive_Median_Filter);
    namedWindow("dst_opencv_medianBlur", WINDOW_AUTOSIZE);
    imshow("dst_opencv_medianBlur", dst_opencv_medianBlur);
    namedWindow("dst_adaptiveProcess", WINDOW_AUTOSIZE);
    imshow("dst_adaptiveProcess", dst_adaptiveProcess);
    waitKey(0);
    return 0;
       
}
