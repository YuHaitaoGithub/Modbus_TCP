#include "..\include\main.h"



/*��Ȧ����֡������*/
void Coilrw(uint8_t* in_num,int* retlen, uint8_t* Rdata)
{
	int functionCode = in_num[7] & 0xff;//������
	int StrAddress = ((in_num[8] & 0xff) << 8) | (in_num[9] & 0xff);//��ʼ��ַ
	int RstNum = ((in_num[10] & 0xff) << 8) | (in_num[11] & 0xff);//��Ȧ����
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
		/*01������*************/
		if (functionCode == 1)
		{
			for (int i = StrAddress; j < 8 && RstNum; i++, j++)
			{
				RstNum--;
				char k[10] = {};
				_itoa_s(i, k, 10);
				num = GetPrivateProfileIntA(CoilSection, k, -1, CoilFilename);
				char *pp = CoilFilename;
				in_num[*retlen] = ((0xff & num) << (j)) | in_num[*retlen];
			}
			*retlen = *retlen + 1;
			if (!RstNum)break;
			j = 0;
			StrAddress = StrAddress + 8;
		}
		/*15������**********/
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
	return;
}


/*�Ĵ����Ķ�д*/
void Regist(uint8_t* ret_num, int* retlenth, uint8_t* Reivedata)
{
	int functionCode = ret_num[7] & 0xff;
	int StrAddress = ((ret_num[8] & 0xff) << 8) | (ret_num[9] & 0xff);
	int RstNum = ((ret_num[10] & 0xff) << 8) | (ret_num[11] & 0xff);
	int num = 0;
	int d = RstNum * 2;
	/*03������*/
	if (functionCode == 3)
	{
		memset(ret_num + 8, 0, 1015);
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
	}
	/*16������*/
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



void DataJuage(uint8_t* Rdata, int* Rlen)
{
	MBAPheadJuage numJuage;
	uint16_t code = 0;//�쳣��
	int address = 0;//��ʼ��ַ
	int number = 0;//Ҫ�жϵ�����
	int Bytenum = 0;//�ֽ���
	uint16_t function = ((uint16_t)Rdata[7]);//������

	/*01�쳣���ж�*/
	if ((function == 1 || function == 3 || function == 15 || function == 16))
	{
		address = (uint16_t)(((uint16_t)((Rdata[8]) << 8)) | ((uint16_t)Rdata[9]));
		number = (uint16_t)(((uint16_t)((Rdata[10]) << 8)) | ((uint16_t)Rdata[11]));
		if (1 == function || 3 == function)
		{
			/*01���������ݷ�Χ�ж�*/
			if (1 == function)
			{
				if (number > 2000 || number < 1 || *Rlen != 12)
				{
					cout << "�Ƿ�����ֵ,�����쳣��03" << endl;
					code = 3;
					numJuage.DataEorry(Rdata, Rlen, code, function);
					return;
				}
				if (number + address > 2500)
				{
					cout << "�Ƿ����ݵ�ַ,�����쳣��02" << endl;
					code = 2;
					numJuage.DataEorry(Rdata, Rlen, code, function);
					return;
				}
				return;
			}

			/*03���������ݷ�Χ�ж�*/
			if (number > 125 || number < 1 || *Rlen != 12)
			{
				cout << "�Ƿ�����ֵ,�����쳣��03" << endl;
				code = 3;
				numJuage.DataEorry(Rdata, Rlen, code, function);
				return;
			}
			if (number + address > 250)
			{
				cout << "�Ƿ����ݵ�ַ,�����쳣��02" << endl;
				code = 2;
				numJuage.DataEorry(Rdata, Rlen, code, function);
				return;
			}
			return;
		}

		/*15���������ݷ�Χ�ж�*/
		if (function == 15 || function == 16)
		{
			Bytenum = (*Rlen) == 12 ? 0 : Rdata[12] & 0xff;
			int m = (number + 7) / 8;
			if (15 == function)
			{
				if (number > 1968 || number < 1 || Bytenum != (*Rlen) - 13 || Bytenum == 0 || m != Bytenum)
				{
					cout << "�Ƿ�����ֵ,�����쳣��03" << endl;
					code = 3;
					numJuage.DataEorry(Rdata, Rlen, code, function);
					return;
				}
				if (number + address > 2500)
				{
					cout << "�Ƿ����ݵ�ַ,�����쳣��02" << endl;
					code = 2;
					numJuage.DataEorry(Rdata, Rlen, code, function);
					return;
				}
				return;
			}

			/*16���������ݷ�Χ�ж�*/
			m = number * 2;
			if (number > 123 || number < 1 || Bytenum != (*Rlen) - 13 || Bytenum == 0 || m != Bytenum)
			{
				cout << "�Ƿ�����ֵ,�����쳣��03" << endl;
				code = 3;
				numJuage.DataEorry(Rdata, Rlen, code, function);
				return;
			}
			if (number + address > 250)
			{
				cout << "�Ƿ����ݵ�ַ,�����쳣��02" << endl;
				code = 2;
				numJuage.DataEorry(Rdata, Rlen, code, function);
				return;
			}
			return;
		}
	}
	else {
		if (function >= 128)
			cout << "���������" << endl;
		else
			cout << "�Ƿ�������,�����쳣��01" << endl;
		code = 1;
		numJuage.DataEorry(Rdata, Rlen, code, function);
		return;
	}
}