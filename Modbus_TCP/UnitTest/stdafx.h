// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

// ���� CppUnitTest ��ͷ�ļ�
#include "CppUnitTest.h"
#include"..\\Modbus_TCP\\include\\MBAPjuage.h"
#include"..\\Modbus_TCP\\include\\main.h"
#include <winsock2.h>




// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#pragma comment(lib,"ws2_32.lib")

void DataTest();