#define _CRT_SECURE_NO_WARNINGS
#include "..\include\Data_Transformation.h"

SystemChange::SystemChange()
{

}

SystemChange::~SystemChange()
{

}

uint16_t SystemChange::ChangeNum(uint8_t* str)
{
	int   num[16] = { 0 };
	int   count = 1;
	uint16_t   result = 0;
	for (int i = 1; i >= 0; i--)
	{
		if ((str[i] >= '0') && (str[i] <= '9'))
			num[i] = str[i] - 48;//字符0的ASCII值为48
		else if ((str[i] >= 'a') && (str[i] <= 'f'))
			num[i] = str[i] - 'a' + 10;
		else if ((str[i] >= 'A') && (str[i] <= 'F'))
			num[i] = str[i] - 'A' + 10;
		else
			num[i] = 0;
		result = result + num[i] * count;
		count = count * 16;//十六进制(如果是八进制就在这里乘以8)    
	}
	return result;
}



void SystemChange::nToHexstr(uint8_t n, uint8_t * hexstr, uint8_t strlen)
{
	uint8_t hexChar[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	uint8_t i;
	uint8_t dis;
	unsigned long nTemp = (long)n;
	for (i = 1; i <= strlen; i++)
	{
		dis = nTemp & 0x0f;
		hexstr[strlen - i] = hexChar[dis];
		nTemp = nTemp >> 4;
	}
	hexstr[strlen] = '\0';
}


uint16_t SystemChange::ReceiveLenth(uint8_t* data)
{
	uint16_t d = 0x00ff & data[1];
	switch (d)
	{
	case 1:{
		d = (((data[4] & 0x00ff)<<8)| (data[5] & 0x00ff));
		d = d % 8 == 0 ? d / 8 : d / 8 + 1;
		d += 5;
		break;
	}
	case 3:{
		d = (((data[4] & 0x00ff) << 8) | (data[5] & 0x00ff));
		d = d * 2 + 5;
		break;
	}
	case 15:{d = 8;break;}
	case 16:{d = 8;break;}
	}
	return d;
}


bool SystemChange::MBAPhead_Juage(uint8_t* Rdata, int Rlen)
{
	if (((uint16_t)Rdata[2]) | ((uint16_t)Rdata[3]) != 0)
	{
		cout << "协议标识错误" << endl;
		return false;
	}
	uint16_t d = ((((uint16_t)Rdata[4]) << 8) | ((uint16_t)Rdata[5]));
	if (Rlen - 6 != d)
	{
		cout << "字节数错误" << endl;
		return false;
	}
	if (Rdata[6] != AddRess)
	{
		cout << "单元标识符错误" << endl;
		return false;
	}
	return true;
}


void SystemChange::DataEorry(uint8_t* &Rdata, int* Rlen, uint16_t code, uint16_t function)
{
	int Retlen = 0;
	function += 0x80;
	Rdata[7] = function;
	Rdata[8] = code & 0xff;
	Retlen = 9;
	memset(Rdata + 9, 0, *Rlen - 9);
	*Rlen = Retlen;
	return;
}