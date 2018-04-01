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
                             ver1.2, 2018.03.30, William Yu, add Q
				                Q1.函数内部没有做卷积核归一化,使用此函数之前一定记得归一化。
						    saturate_cast是为了和opencv内置filter2D函数作效果对比，
						    因为使用了saturate_cast做限定，所以如果采用在使用此函数之前不做归一化，而对输出图像做规定化的方案，是没有用的。
				                Q2.卷积函数只能做3阶卷积（solved）
                             ver1.3, 2018.03.31, William Yu, Q2 solved
=====================================================================================*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

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
	  int temp = 0;
	  for(int aaa=0; aaa<ksize; ++aaa) 
	  {
	     for(int bbb=0; bbb<ksize; ++bbb)
	     {
		temp += kernel[aaa][bbb] * col_ptrs[aaa][i - nChannels*center + nChannels*bbb];
	     }
	  }
	  ///saturate_cast函数 ： a>255则a=255,a<0，则a=0
	  *output++ = saturate_cast<uchar>(temp); 
	}
    }
}

static void help()
{
    cout << "\n这是一个演示数字图像处理模板卷积的程序"
            "\n使用OpenCV version " << CV_VERSION << endl;
    cout << "\n使用:\n"
            "\t./模板卷积 [图片名称 -- 默认值为 ubuntu.png]\n\n";
	
    cout << "\nThis is a demo of ,"
            "\nUsing OpenCV version " << CV_VERSION << endl;
    cout << "\nCall:\n"
            "    ./模板卷积 [image_name -- Default is ubuntu.png]\n\n";
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
    
  
    int ksize = 5;
    //<<输入到矩阵Mat_<double> 中，然后隐式转换成Mat类型, 提供给filter2D函数使用
    Mat matkernel = (Mat_<double>(ksize,ksize) << 
		 0, -1, -2, -1,  0,
		-1, -2, -3, -2, -1,
		-2, -3, 37, -3  -2,
		-1, -2, -3, -2, -1,
		 0, -1, -2, -1,  0);
    ///建立一个size*size大小的动态二维数组 
    double** kernel = new double*[ksize];
    for (int i = 0; i < ksize; ++i)  
        kernel[i] = new double[ksize];
    //赋值，提供给myconvolution函数使用
    for (int i=0;i<ksize;i++)
      for(int j=0;j<ksize;j++)
	kernel[i][j] = matkernel.at<double>(i,j);

    //--------------------------------【 myconvolution()函数 】----------------------------------------------
    //		卷积计算
    //-------------------------------------------------------------------------------------------------
    myconvolution(srcImage, dstImageMyCon, kernel, ksize);  
    //保证安全,归定化到0~255  
    //normalize(dstImageMyCon, dstImageMyCon, 0, 255, CV_MINMAX);

    //--------------------------------【 filter2D()函数 】----------------------------------------------
    //         opencv提供的卷积操作函数
    //-------------------------------------------------------------------------------------------------
    filter2D(srcImage, dstImageOpencv, srcImage.depth(), matkernel); 
    
    namedWindow("srcImage", WINDOW_AUTOSIZE);
    imshow("srcImage", srcImage);
    namedWindow("dstImageMyCon",WINDOW_AUTOSIZE);
    imshow("dstImageMyCon",dstImageMyCon);
    namedWindow("dstImageOpencv", WINDOW_AUTOSIZE);
    imshow("dstImageOpencv", dstImageOpencv);
    waitKey(0);
    return 0;
}
