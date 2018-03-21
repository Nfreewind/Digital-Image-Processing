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
=====================================================================================*/

g++编译:
g++  模板卷积.cpp -o 模板卷积  `pkg-config  --cflags --libs opencv`
Usage:
./模板卷积 -h                        | 查看帮助信息
./模板卷积 [输入参数图片名字]         | 如：./模板卷积 ubuntu.png ,无参数时默认使用ubuntu.png
