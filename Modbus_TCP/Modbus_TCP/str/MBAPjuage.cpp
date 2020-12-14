#define _CRT_SECURE_NO_WARNINGS
#include "..\include\MBAPJuage.h"

MBAPheadJuage::MBAPheadJuage()
{

}

MBAPheadJuage::~MBAPheadJuage()
{

}


/*MBAP头的判断*/
bool MBAPheadJuage::MBAPhead_Juage(uint8_t* Rdata, int Rlen)
{
	if (Rlen < 12)
	{
		cout << "数据长度不合法" << endl;
		return false;
	}
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

/*异常码生成*/
void MBAPheadJuage::DataEorry(uint8_t* &Rdata, int* Rlen, uint16_t code, uint16_t function)
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


uint16_t MBAPheadJuage::ChangeNum(uint8_t* str)
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
		
		result = result + num[i] * count;
		count = count * 16;//十六进制(如果是八进制就在这里乘以8)    
	}
	return result;
}