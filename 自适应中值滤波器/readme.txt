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
                             ver1.1, 2018.03.22, William Yu, add notes
=====================================================================================*/

g++编译:
g++  Median_filter.cpp -o Median_filter  `pkg-config  --cflags --libs opencv`
Usage:
./Median_filter -h                        | 查看帮助信息
./Median_filter [输入参数图片名字]         | 如：./Median_filter ubuntu.png ,无参数时默认使用ubuntu.png
