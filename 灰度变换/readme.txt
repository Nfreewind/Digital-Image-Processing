/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 灰度变换.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *			
 * Reference               : http://www.cnblogs.com/wangguchangqing/p/6983680.html
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.13, William Yu
                             ver1.1, 2018.03.13, William Yu, add notes
=====================================================================================*/

g++编译:
g++  灰度变换.cpp -o 灰度变换  `pkg-config  --cflags --libs opencv`
Usage:
./灰度变换 -h                        | 查看帮助信息
./灰度变换 [输入参数图片名字]         | 如：./灰度变换 ubuntu.png ,无参数时默认使用ubuntu.png
