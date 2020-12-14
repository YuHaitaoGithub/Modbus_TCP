// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

// 用于 CppUnitTest 的头文件
#include "CppUnitTest.h"
#include"..\\Modbus_TCP\\include\\MBAPjuage.h"
#include"..\\Modbus_TCP\\include\\main.h"
#include <winsock2.h>




// TODO:  在此处引用程序需要的其他头文件

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#pragma comment(lib,"ws2_32.lib")

void DataTest();