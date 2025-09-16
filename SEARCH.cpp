#pragma once

#include<iostream>
#include"Searching.h"







int main()////////////////////////////////注释1
{
	std::cout << "Hello World!\n";


	//调用示例
	Route a;
	Time t(2025, 8, 23, 8, 30, 6);
	std::string str1 = "东北门";
	std::string str2 = "无线谷";
	a.Search(t, str1, str2);
	std::cout << a.t1.clock << ':' << a.t1.min<<'\n';
	std::cout << a.t_t1.clock << ':' << a.t_t1.min<<'\n';
	std::cout << a.t_t2.clock << ':' << a.t_t2.min<<'\n';
	std::cout << a.t2.clock << ':' << a.t2.min<<'\n';
	//之后可以用a的任意成员进行赋值输出等操作

}
