#include "..\include\main.h"


void Registe(uint8_t* in_num, int* ret,uint8_t* Rdata)
{
	//SystemChange data;
	//uint8_t l[10] = {};
	//uint16_t num = 0;
	//uint16_t num2 = 0;

	//for (int i = 0; i < 8; i++)
	//{
	//	in_num[i] = Rdata[i];
	//}

	//num2 = Rdata[7]&0xff;
	//switch (num2)
	//{
	//case 1:{
	//	cout << "输入线圈起始地址(十进制整数)" << endl;
	//	cin >> num;
	//	in_num[(*ret)++] = (num >> 8) & 0xff;
	//	in_num[(*ret)++] = num & 0xff;

	//	cout << "要读线圈个数（十进制整数）" << endl;
	//	cin >> num;
	//	in_num[(*ret)++] = (num >> 8) & 0xff;
	//	in_num[(*ret)++] = num & 0xff;
	//	break;
	//}
	//case 3:{
	//	cout << "输入寄存器起始地址（十进制整数）" << endl;
	//	cin >> num;
	//	in_num[(*ret)++] = (num >> 8) & 0xff;
	//	in_num[(*ret)++] = num & 0xff;

	//	cout << "要读寄存器的个数(十进制整数)" << endl;
	//	cin >> num;
	//	in_num[(*ret)++] = (num >> 8) & 0xff;
	//	in_num[(*ret)++] = num & 0xff;
	//	break;
	//}
	//case 15:{
	//	cout << "输入线圈起始地址(十进制整数)" << endl;
	//	cin >> num;
	//	in_num[(*ret)++] = (num >> 8) & 0xff;
	//	in_num[(*ret)++] = num & 0xff;

	//	cout << "要写入的线圈个数(十进制整数)" << endl;
	//	cin >> num;
	//	in_num[(*ret)++] = (num >> 8) & 0xff;
	//	in_num[(*ret)++] = num & 0xff;

	//	num = num % 8 != 0 ? num = num / 8 + 1 : num /= 8;
	//	in_num[(*ret)++] = num & 0xff;
	//	while (num--)
	//	{
	//		uint16_t i = num + 1;
	//		printf("要写入第%d个字节的数值(十六进制)\n", i - num);
	//		cin >> l;
	//		in_num[(*ret)++] = 0xff & data.ChangeNum(l);
	//		memset(l, 0, 10);
	//	}
	//	break;
	//}
	//case 16:{
	//	cout << "输入寄存器起始地址(十进制整数)" << endl;
	//	cin >> num;
	//	in_num[(*ret)++] = (num >> 8) & 0xff;
	//	in_num[(*ret)++] = num & 0xff;

	//	cout << "要写入寄存器的个数(十进制整数)" << endl;
	//	cin >> num;
	//	in_num[(*ret)++] = (num >> 8) & 0xff;
	//	in_num[(*ret)++] = num & 0xff;
	//	if (num >= 2)num /= 2;
	//	while (num--)
	//	{
	//		uint16_t i = num + 1;
	//		printf("要写入第%d个寄存器的数值的高位(十六进制)\n", i - num);
	//		cin >> l;
	//		in_num[(*ret)++] = data.ChangeNum(l) & 0xff;
	//		memset(l, 0, 10);
	//		printf("要写入第%d个寄存器的数值的低位(十六进制)\n", i - num);
	//		cin >> l;
	//		in_num[(*ret)++] = data.ChangeNum(l) & 0xff;
	//		memset(l, 0, 10);
	//	}
	//	break;
	//}
	return;
}