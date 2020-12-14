#define _CRT_SECURE_NO_WARNINGS
#include "..\include\MBAPJuage.h"

MBAPheadJuage::MBAPheadJuage()
{

}

MBAPheadJuage::~MBAPheadJuage()
{

}


/*MBAPͷ���ж�*/
bool MBAPheadJuage::MBAPhead_Juage(uint8_t* Rdata, int Rlen)
{
	if (Rlen < 12)
	{
		cout << "���ݳ��Ȳ��Ϸ�" << endl;
		return false;
	}
	if (((uint16_t)Rdata[2]) | ((uint16_t)Rdata[3]) != 0)
	{
		cout << "Э���ʶ����" << endl;
		return false;
	}
	uint16_t d = ((((uint16_t)Rdata[4]) << 8) | ((uint16_t)Rdata[5]));
	if (Rlen - 6 != d)
	{
		cout << "�ֽ�������" << endl;
		return false;
	}
	if (Rdata[6] != AddRess)
	{
		cout << "��Ԫ��ʶ������" << endl;
		return false;
	}
	return true;
}

/*�쳣������*/
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
			num[i] = str[i] - 48;//�ַ�0��ASCIIֵΪ48
		else if ((str[i] >= 'a') && (str[i] <= 'f'))
			num[i] = str[i] - 'a' + 10;
		else if ((str[i] >= 'A') && (str[i] <= 'F'))
			num[i] = str[i] - 'A' + 10;
		
		result = result + num[i] * count;
		count = count * 16;//ʮ������(����ǰ˽��ƾ����������8)    
	}
	return result;
}