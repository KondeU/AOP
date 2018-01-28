/************************************************************

GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
Copyright (c) 2017, KondeU, All rights reserved.

Project:     AOP
File:        Demo.cpp
Description: A demo show the C++ Aspect-Oriented Programming
Date:        2017-12-28
Version:     1.0
Authors:     Deyou Kong <370242479@qq.com>
History:     01, 17-12-28, Deyou Kong, Create file and implement it.

************************************************************/

#include <iostream>
using namespace std;

#include "AOP.hpp"

struct AspectDemo
{
	void Before(...)
	{
		iCounter++;
		cout << "Aspect : Before, counter is " << iCounter << endl;
	}

	void After(...)
	{
		iCounter++;
		cout << "Aspect : After, counter is " << iCounter << endl;
	}

private:

	int iCounter = 0;
};

double Demo1(int i)
{
	cout << "Demo1 run, input int value is "<< i << "." << endl;
	cout << "Demo1 has a return value with type is double and value is -1.5." << endl;
	return -1.5;
}

bool Demo2()
{
	cout << "Demo2 run, without input." << endl;
	cout << "Demo2 has a return value with type is bool and value is true." << endl;
	return true;
}

int Demo3()
{
	cout << "Demo3 run, without input." << endl;
	cout << "Demo3 has a return value with type is int and value is -1." << endl;
	return -1;
}

int main()
{
	cout << "AOP called : Demo1 : " << endl;
	cout << "Ret: " << (AOP<double, AspectDemo>(Demo1, 12)) << endl;
	cout << endl;

	cout << "AOP called : Demo2 : " << endl;
	cout << "Ret:" << (AOP<bool, AspectDemo, AspectDemo>(Demo2)) << endl;
	cout << endl;

	cout << "Call directly : Demo3 : " << endl;
	Demo3();
	cout << endl;

	system("pause");
	return 0;
}
