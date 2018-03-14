/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 比特平面分层.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *			
 * Reference               : http://blog.csdn.net/liuchuan__________/article/details/48520655
 * 		         	http://blog.csdn.net/qq_19332527/article/details/78331240
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.13, William Yu
                             ver1.1, 2018.03.13, William Yu, add notes
=====================================================================================*/

g++编译:
g++  比特平面分层.cpp -o 比特平面分层  `pkg-config  --cflags --libs opencv`
Usage:
./比特平面分层 -help                     | 查看帮助信息
./比特平面分层 [输入参数图片名字]         | 如：./比特平面分层 ubuntu.png ,无参数时默认使用ubuntu.png
