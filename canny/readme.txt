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
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.24, William Yu
                             ver1.1, 2018.03.24, William Yu, add notes
=====================================================================================*/

g++编译:
g++  canny.cpp -o canny  `pkg-config  --cflags --libs opencv`
Usage:
./canny -h                        | 查看帮助信息
./canny [输入参数图片名字]         | 如：./canny ubuntu.png ,无参数时默认使用ubuntu.png
