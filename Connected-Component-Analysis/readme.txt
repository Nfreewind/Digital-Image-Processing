/*=====================================================================================
 *                Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 搜索连通域.cpp
 * Description             : (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 *				种子填充法和两遍扫描法检索连通域      
 * Reference               : http://blog.csdn.net/icvpr/article/details/10259577
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.02.28, William Yu
                             ver1.1, 2018.03.01, William Yu, add notes
=====================================================================================*/

g++编译
g++  搜索连通域.cpp -o search  `pkg-config  --cflags --libs opencv`

Usage:
./search -help                     | 查看帮助信息
