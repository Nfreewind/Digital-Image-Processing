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

g++编译:
g++  Fourier_transform.cpp -o Fourier_transform  `pkg-config  --cflags --libs opencv`
Usage:
./Fourier_transform -h                        | 查看帮助信息
./Fourier_transform [输入参数图片名字]         | 如：./Fourier_transform ubuntu.png ,无参数时默认使用ubuntu.png




/*=====================================================================================
 * Filename                : dft.cpp
 * Description             : opencv_source_code/samples/cpp/dft.cpp
				对一张图片做离散傅里叶变换
 * Programmer(s)           : opencv
 * Modification History	   : ver1.0, opencv3.3.1
                             ver1.1, 2018.03.27, William Yu, add notes
=====================================================================================*/
