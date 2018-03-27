/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : Gaussian_filter.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *			
 * Reference               : https://docs.opencv.org/3.3.1/d4/d86/group__imgproc__filter.html#gaabe8c836e97159a9193fb0b11ac52cf1
 *				https://docs.opencv.org/3.3.1/d4/d86/group__imgproc__filter.html#gac05a120c1ae92a6060dd0db190a61afa
 * 				https://docs.opencv.org/3.3.1/de/db2/laplace_8cpp-example.html#a17
 *				http://blog.csdn.net/weixin_37720172/article/details/72843238,,,,,,,该文档代码怀疑有错误,已与作者联系
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.22, William Yu
                             ver1.1, 2018.03.22, William Yu, add notes
=====================================================================================*/

g++编译:
g++  Gaussian_filter.cpp -o Gaussian_filter  `pkg-config  --cflags --libs opencv`
Usage:
./Gaussian_filter -h                        | 查看帮助信息
./Gaussian_filter [输入参数图片名字]         | 如：./Gaussian_filter ubuntu.png ,无参数时默认使用ubuntu.png
