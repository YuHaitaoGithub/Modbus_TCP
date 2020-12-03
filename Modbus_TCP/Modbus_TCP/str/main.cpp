#include"..\include\main.h"


void DataJuage(uint8_t* Rdata, int* Rlen)
{
	SystemChange numJuage;
	uint16_t code = 0;
	int address = 0;
	int number = 0;
	int Bytenum = 0;
	uint16_t function = ((uint16_t)Rdata[7]);
	if ((function == 1 || function == 3 || function == 15 || function == 16))
	{
		address = (uint16_t)(((uint16_t)((Rdata[8]) << 8)) | ((uint16_t)Rdata[9]));
		number = (uint16_t)(((uint16_t)((Rdata[10]) << 8)) | ((uint16_t)Rdata[11]));
		if (address > 65535)
		{
			cout << "非法数据地址,返回异常码02" << endl;
			code = 2;
			numJuage.DataEorry(Rdata, Rlen, code, function);
			return;
		}
		if (1 == function || 3 == function)
		{
			if (1 == function)
			{
				if (number > 2000 || number < 1 || number + address > 2000)
				{
					cout << "非法数据值,返回异常码03" << endl;
					code = 3;
					numJuage.DataEorry(Rdata, Rlen, code, function);
					return;
				}
				return;
			}
			if (number > 125 || number < 1 || number + address > 125)
			{
				cout << "非法数据值,返回异常码03" << endl;
				code = 3;
				numJuage.DataEorry(Rdata, Rlen, code, function);
			}
			return;
		}
		if (function == 15 || function == 16)
		{
			Bytenum = Rdata[12] & 0xff;
			if (15 == function)
			{
				if (number > 1968 || number < 1 || number + address > 1968 || Bytenum != (*Rlen)-13)
				{
					cout << "非法数据值,返回异常码03" << endl;
					code = 3;
					numJuage.DataEorry(Rdata, Rlen, code, function);
					return;
				}
				return;
			}
			if (number > 123 || number < 1 || number + address > 123 || Bytenum != (*Rlen) - 13)
			{
				cout << "非法数据值,返回异常码03" << endl;
				code = 3;
				numJuage.DataEorry(Rdata, Rlen, code, function);
			}
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



int main(int argc, char* argv[])
{
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//绑定IP和端口
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(502);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)& sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}


	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}


	//循环接收数据
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	uint8_t revData[260];
	while (true)
	{
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR*)& remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
		for (;;)
		{
			if (sClient == NULL)
			{
				closesocket(sClient);
				break;
			}
			int ret = recv(sClient, (char*)revData, 260, 0);
			/*接受错误判断********/
			if (ret == SOCKET_ERROR)
			{
				cout << "接收错误" << endl;
				memset(revData, 0, sizeof(revData));
				//closesocket(sClient);
				break;
			}

			/*网口检测**********/
			if (ret == 0)
			{
				cout << "网络中断" << endl;
				memset(revData, 0, sizeof(revData));
				//closesocket(sClient);
				break;
			}

			/*长度判断***********/
			if (ret < 12)
			{
				cout << "数据长度不合法" << endl;
				memset(revData, 0, sizeof(revData));
				//closesocket(sClient);
				continue;
			}

			SystemChange dataJuage;
			if (!dataJuage.MBAPhead_Juage(revData, ret))
			{
				memset(revData, 0, sizeof(revData));
				//closesocket(sClient);
				continue;
			}

			int Rlen = ret;
			DataJuage(revData, &Rlen);
			if (Rlen != ret)
			{
				send(sClient, (const char*)(&revData), Rlen, 0);
				memset(revData, 0, sizeof(revData));
				cout << "已发送异常码" << endl;
				//closesocket(sClient);
				continue;
			}

			int retlen = 12;

			if ((uint16_t)(revData[7] & 0xff) == 1 || (uint16_t)(revData[7] & 0xff) == 15)
			{
				uint8_t Coil[500] = {};
				memcpy(Coil, revData, 12);
				Coilrw(Coil, &retlen, revData);
				Coil[4] = ((retlen - 6) >> 8) & 0xff;
				Coil[5] = (retlen - 6) & 0xff;
				send(sClient, (const char*)(&Coil), retlen, 0);
				cout << "已发送线圈" << endl;
				for (int i = 0; i < retlen; i++)
				{
					uint8_t h[5] = {};
					dataJuage.nToHexstr(Coil[i], h, 2);
					cout << h << " ";
				}
				memset(Coil, 0, sizeof(Coil));
				cout << endl;
			}

			else
			{
				uint8_t Register[500] = {};
				memcpy(Register, revData, 12);
				Regist(Register, &retlen, revData);
				Register[4] = ((retlen - 6) >> 8) & 0xff;
				Register[5] = (retlen - 6) & 0xff;
				send(sClient, (const char*)(&Register), retlen, 0);
				cout << "已发送寄存器" << endl;
				for (int i = 0; i < retlen; i++)
				{
					uint8_t h[5] = {};
					dataJuage.nToHexstr(Register[i], h, 2);
					cout << h << " ";
				}
				memset(Register, 0, sizeof(Register));
				cout << endl;
			}
			memset(revData, 0, sizeof(revData));
		}
	}
	closesocket(slisten);
	WSACleanup();
	return 0;
}
