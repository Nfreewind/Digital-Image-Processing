/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : equalizeHist.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *				对一个灰度图直方图均衡化  
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.12, William Yu
                             ver1.1, 2018.03.12, William Yu, add notes
=====================================================================================*/

g++编译:
g++  equalizeHist.cpp -o equalizeHist  `pkg-config  --cflags --libs opencv`

Usage:
./equalizeHist -help                     | 查看帮助信息
./equalizeHist [输入参数图片名字]         | 如：./equalizeHist ubuntu.png ,无参数时默认使用ubuntu.png
