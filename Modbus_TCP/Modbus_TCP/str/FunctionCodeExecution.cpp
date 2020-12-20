#include "..\include\main.h"



/*线圈返回帧的生成*/
void Coilrw(uint8_t* in_num,int* retlen, uint8_t* Rdata,uint8_t *lpcoil)
{
	int functionCode = in_num[7] & 0xff;//功能码
	int StrAddress = ((in_num[8] & 0xff) << 8) | (in_num[9] & 0xff);//起始地址
	int RstNum = ((in_num[10] & 0xff) << 8) | (in_num[11] & 0xff);//线圈个数
	int num = 0;
	int d = RstNum % 8 == 0 ? RstNum / 8 : RstNum / 8 + 1;
	int j = 0;

	if (functionCode == 1)
	{
		memset(in_num + 8, 0, 1015);
		in_num[8] = 0xff & d;
		*retlen = 9;
	}
	int len = *retlen + 1;
	while (d--)
	{
		/*01功能码*************/
		if (functionCode == 1)
		{
			for (int i = StrAddress; j < 8 && RstNum; i++, j++)
			{
				RstNum--;
				in_num[*retlen] = ((0xff & lpcoil[i]) << (j)) | in_num[*retlen];
			}
			*retlen = *retlen + 1;
			if (!RstNum)break;
			j = 0;
			StrAddress = StrAddress + 8;
		}
		/*15功能码**********/
		else
		{
			for (int i = StrAddress; j < 8 && RstNum; i++, j++)
			{
				RstNum--;
				lpcoil[i] = (Rdata[len] >> j) & 0x01;
			}
			++len;
			if (!RstNum)break;
			j = 0;
			StrAddress = StrAddress + 8;
		}
	}
	return;
}


/*寄存器的读写*/
void Regist(uint8_t* ret_num, int* retlenth, uint8_t* Reivedata,uint8_t *lprest)
{
	int functionCode = ret_num[7] & 0xff;
	int StrAddress = ((ret_num[8] & 0xff) << 8) | (ret_num[9] & 0xff);
	int RstNum = ((ret_num[10] & 0xff) << 8) | (ret_num[11] & 0xff);
	int num = 0;
	int d = RstNum * 2;
	/*03功能码*/
	if (functionCode == 3)
	{
		memset(ret_num + 8, 0, 1015);
		ret_num[8] = 0xff & d;
		*retlenth = 9;
		for (int i = StrAddress; i < RstNum + StrAddress; i++, *retlenth = (*retlenth) + 2)
		{
			ret_num[*retlenth] = (0xff & (lprest[i] >> 8));
			ret_num[(*retlenth) + 1] = 0xff & lprest[i];
		}
	}
	/*16功能码*/
	else{
		int len = *retlenth + 1;
		for (int i = StrAddress; i < RstNum + StrAddress; i++, len = len + 2)
			lprest[i] = ((Reivedata[len] & 0xff) << 8) | (Reivedata[len + 1] & 0xff);
		
	}
	return;
}

void DataJuage_01(uint8_t **lpRdata, int **lpRlen, MBAPheadJuage *lpnumJuage, uint16_t fucn,int number,int address)
{
	int code = 0;
	if (number > 2000 || number < 1 || **lpRlen != 12)
	{
		cout << "非法数据值,返回异常码03" << endl;
		code = 3;
		(*lpnumJuage).DataEorry(*lpRdata, *lpRlen, code, fucn);
		return;
	}
	if (number + address > CoilNum_Max)
	{
		cout << "非法数据地址,返回异常码02" << endl;
		code = 2;
		(*lpnumJuage).DataEorry(*lpRdata, *lpRlen, code, fucn);
		return;
	}
	return;
}

void DataJuage_03(uint8_t **lpRdata, int **lpRlen, MBAPheadJuage *lpnumJuage, uint16_t fucn, int number, int address)
{
	int code = 0;
	if (number > 125 || number < 1 || **lpRlen != 12)
	{
		cout << "非法数据值,返回异常码03" << endl;
		code = 3;
		(*lpnumJuage).DataEorry(*lpRdata, *lpRlen, code, fucn);
		return;
	}
	if (number + address > RestNum_Max)
	{
		cout << "非法数据地址,返回异常码02" << endl;
		code = 2;
		(*lpnumJuage).DataEorry(*lpRdata, *lpRlen, code, fucn);
		return;
	}
	return;
}

void DataJuage_15(uint8_t **lpRdata, int **lpRlen, MBAPheadJuage *lpnumJuage, uint16_t fucn, int number, int address, int Bytenum,int m)
{
	int code = 0;
	if (number > 1968 || number < 1 || Bytenum != (**lpRlen) - 13 || Bytenum == 0 || m != Bytenum)
	{
		cout << "非法数据值,返回异常码03" << endl;
		code = 3;
		(*lpnumJuage).DataEorry(*lpRdata, *lpRlen, code, fucn);
		return;
	}
	if (number + address > CoilNum_Max)
	{
		cout << "非法数据地址,返回异常码02" << endl;
		code = 2;
		(*lpnumJuage).DataEorry(*lpRdata, *lpRlen, code, fucn);
		return;
	}
	return;
}


void DataJuage_16(uint8_t **lpRdata, int **lpRlen, MBAPheadJuage *lpnumJuage, uint16_t fucn, int number, int address, int Bytenum, int m)
{
	int code = 0;
	if (number > 123 || number < 1 || Bytenum != (**lpRlen) - 13 || Bytenum == 0 || m != Bytenum)
	{
		cout << "非法数据值,返回异常码03" << endl;
		code = 3;
		(*lpnumJuage).DataEorry(*lpRdata, *lpRlen, code, fucn);
		return;
	}
	if (number + address > RestNum_Max)
	{
		cout << "非法数据地址,返回异常码02" << endl;
		code = 2;
		(*lpnumJuage).DataEorry(*lpRdata, *lpRlen, code, fucn);
		return;
	}
	return;
}



void DataJuage(uint8_t* Rdata, int* Rlen)
{
	MBAPheadJuage numJuage;
	uint16_t code = 0;//异常码
	int address = 0;//起始地址
	int number = 0;//要判断的数据
	int Bytenum = 0;//字节数
	uint16_t function = ((uint16_t)Rdata[7]);//功能码

	if ((function == 1 || function == 3 || function == 15 || function == 16))
	{
		address = (uint16_t)(((uint16_t)((Rdata[8]) << 8)) | ((uint16_t)Rdata[9]));
		number = (uint16_t)(((uint16_t)((Rdata[10]) << 8)) | ((uint16_t)Rdata[11]));
		/*01功能码数据范围判断*/
		if (1 == function)
		{
			DataJuage_01(&Rdata, &Rlen, &numJuage, function, number, address);
			return;
		}

		/*03功能码数据范围判断*/
		if (function == 3)
		{
			DataJuage_03(&Rdata, &Rlen, &numJuage, function, number, address);
			return;
		}
		
		Bytenum = (*Rlen) == 12 ? 0 : Rdata[12] & 0xff;
		int m = 0;
		if (15 == function)
		{
			m = (number + 7) / 8;
			DataJuage_15(&Rdata, &Rlen, &numJuage, function, number, address, Bytenum, m);
			return;
		}
		/*16功能码数据范围判断*/
		if (function == 16)
		{
			m = number * 2;
			DataJuage_16(&Rdata, &Rlen, &numJuage, function, number, address, Bytenum, m);
			return;
		}
	}
	else {
		if (function >= 128)
			cout << "功能码过大" << endl;
		else
			cout << "非法功能码,返回异常码01" << endl;
		code = 1;
		numJuage.DataEorry(Rdata, Rlen, code, function);
		return;
	}
}