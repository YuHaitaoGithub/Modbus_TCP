#include "..\include\main.h"


void Coilrw(uint8_t* in_num,int* retlen, uint8_t* Rdata)
{
	int functionCode = in_num[7] & 0xff;
	char *filename = "./Coil.ini";
	char *section = "Coil";
	int StrAddress = ((in_num[8] & 0xff) << 8) | (in_num[9] & 0xff);
	int RstNum = ((in_num[10] & 0xff) << 8) | (in_num[11] & 0xff);
	int num = 0;
	int d = RstNum % 8 == 0 ? RstNum / 8 : RstNum / 8 + 1;
	int j = 0;
	while (d--)
	{
		if (functionCode == 1)
		{
			in_num[(*retlen)] = 0xff & d;
			*retlen = (*retlen) + 1;
			for (int i = StrAddress; j < 8 && RstNum; i++, j++)
			{
				RstNum--;
				char k[5] = {};
				_itoa_s(i, k, 10);
				num = GetPrivateProfileIntA(section, k, -1, filename);
				in_num[*retlen] = ((0xff & num) << (j)) | in_num[*retlen];
			}
			*retlen = *retlen + 1;
			if (!RstNum)break;
			j = 0;
			StrAddress = StrAddress + 8;
		}
		else
		{
			int len = *retlen;
			for (int i = StrAddress; j < 8 && RstNum; i++, j++)
			{
				RstNum--;
				num = (Rdata[len]>>j) & 0x01;
				char k1[5] = {};
				char k2[5] = {};
				_itoa_s(i, k1, 10);
				_itoa_s(num, k2, 10);
				WritePrivateProfileStringA(section, k1, k2, filename);
			}
			++len;
			if (!RstNum)break;
			j = 0;
			StrAddress = StrAddress + 8;
		}
	}
	return;
}


void Regist(uint8_t* ret_num, int* retlenth, uint8_t* Reivedata)
{
	int functionCode = ret_num[7] & 0xff;
	char *filename = "./Register.ini";
	char *section = "register";
	int StrAddress = ((ret_num[8] & 0xff) << 8) | (ret_num[9] & 0xff);
	int RstNum = ((ret_num[10] & 0xff) << 8) | (ret_num[11] & 0xff);
	int num = 0;
	int d = RstNum * 2;
	if (functionCode == 3)
	{
		ret_num[*retlenth] = 0xff & d;
		*retlenth = (*retlenth) + 1;
		for (int i = StrAddress; i < RstNum; i++, *retlenth = (*retlenth) + 2)
		{
			char k[10] = {};
			_itoa_s(i, k, 10);
			num = GetPrivateProfileIntA(section, k, -1, filename);
			ret_num[*retlenth] = (0xff & (num >> 8));
			ret_num[(*retlenth) + 1] = 0xff & num;
		}
	}
	else{
		int len = *retlenth;
		for (int i = StrAddress; i < RstNum; i++, len = len + 2)
		{
			RstNum--;
			num = ((Reivedata[len] & 0xff)<<8) | (Reivedata[len+1]&0xff);
			char k1[5] = {};
			char k2[5] = {};
			_itoa_s(i, k1, 10);
			_itoa_s(num, k2, 10);
			WritePrivateProfileStringA(section, k1, k2, filename);
		}
	}
	return;
}