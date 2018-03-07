/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : scale.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *				对一个灰度图做标度，均衡像素值在0-255之间    
 * Reference               : http://blog.csdn.net/danieljianfeng/article/details/41807809
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.07, William Yu
                             ver1.1, 2018.03.07, William Yu, add notes
=====================================================================================*/

g++编译
g++  scale.cpp -o scale  `pkg-config  --cflags --libs opencv`
Usage
./scale
