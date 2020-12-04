#include "..\include\main.h"


void Coilrw(uint8_t* in_num,int* retlen, uint8_t* Rdata)
{
	int functionCode = in_num[7] & 0xff;
	//char *CoilFilename = "./Coil.ini";
	//char *CoilSection = "Coil";
	int StrAddress = ((in_num[8] & 0xff) << 8) | (in_num[9] & 0xff);
	int RstNum = ((in_num[10] & 0xff) << 8) | (in_num[11] & 0xff);
	int num = 0;
	int d = RstNum % 8 == 0 ? RstNum / 8 : RstNum / 8 + 1;
	int j = 0;
	if (functionCode == 1)
	{
		memset(in_num + 8, 0, 491);
		in_num[8] = 0xff & d;
		*retlen = 9;
	}
	int len = *retlen + 1;
	while (d--)
	{
		if (functionCode == 1)
		{
			for (int i = StrAddress; j < 8 && RstNum; i++, j++)
			{
				RstNum--;
				char k[5] = {};
				_itoa_s(i, k, 10);
				num = GetPrivateProfileIntA(CoilSection, k, -1, CoilFilename);
				in_num[*retlen] = ((0xff & num) << (j)) | in_num[*retlen];
			}
			*retlen = *retlen + 1;
			if (!RstNum)break;
			j = 0;
			StrAddress = StrAddress + 8;
		}
		else
		{
			for (int i = StrAddress; j < 8 && RstNum; i++, j++)
			{
				RstNum--;
				num = (Rdata[len] >> j) & 0x01;
				char k1[10] = {};
				char k2[10] = {};
				_itoa_s(i, k1, 10);
				_itoa_s(num, k2, 10);
				WritePrivateProfileStringA(CoilSection, k1, k2, CoilFilename);
			}
			++len;
			if (!RstNum)break;
			j = 0;
			StrAddress = StrAddress + 8;
		}
	}
	if ((functionCode == 1)&&(in_num[8] != (*retlen) - 9))
		cout << "线圈数据读取错误" << endl;
	return;
}


void Regist(uint8_t* ret_num, int* retlenth, uint8_t* Reivedata)
{
	int functionCode = ret_num[7] & 0xff;
	int StrAddress = ((ret_num[8] & 0xff) << 8) | (ret_num[9] & 0xff);
	int RstNum = ((ret_num[10] & 0xff) << 8) | (ret_num[11] & 0xff);
	int num = 0;
	int d = RstNum * 2;
	/*03功能码*/
	if (functionCode == 3)
	{
		memset(ret_num + 8, 0, 491);
		ret_num[8] = 0xff & d;
		*retlenth = 9;
		for (int i = StrAddress; i < RstNum + StrAddress; i++, *retlenth = (*retlenth) + 2)
		{
			char k[10] = {};
			_itoa_s(i, k, 10);
			num = GetPrivateProfileIntA(ResSection, k, -1, ResFilename);
			ret_num[*retlenth] = (0xff & (num >> 8));
			ret_num[(*retlenth) + 1] = 0xff & num;
		}
		if (ret_num[8] != (*retlenth) - 9)
			cout << "寄存器数据读取错误" << endl;
	}

	else{
		int len = *retlenth + 1;
		for (int i = StrAddress; i < RstNum + StrAddress; i++, len = len + 2)
		{
			num = ((Reivedata[len] & 0xff)<<8) | (Reivedata[len+1]&0xff);
			char k1[10] = {};
			char k2[10] = {};
			_itoa_s(i, k1, 10);
			_itoa_s(num, k2, 10);
			WritePrivateProfileStringA(ResSection, k1, k2, ResFilename);
		}
	}
	return;
}