#include"..\include\main.h"



int main()
{
	/*初始化线圈和寄存器*/
	for (int m = 0; m < CoilNum_Max; m++)
	{
		char u[10] = {};
		_itoa_s(m, u, 10);
		WritePrivateProfileStringA(CoilSection, u, "0", CoilFilename);
	}
	for (int m = 0; m < RestNum_Max; m++)
	{
		char u[10] = {};
		_itoa_s(m, u, 10);
		WritePrivateProfileStringA(ResSection, u, "0", ResFilename);
	}



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
	uint8_t revData[260];//255+6
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

		u_long mode = 0;
		ioctlsocket(sClient, FIONBIO, &mode);
		
/***********把当前套接字读取完******************/
		for (;;)
		{
			if (sClient == NULL)
			{
				closesocket(sClient);
				break;
			}
		
			int ret = recv(sClient, (char*)revData, 261, 0);

/********************接受错误判断******************/
			if (ret == SOCKET_ERROR)
			{
				cout << "连接中断" << endl;
				memset(revData, 0, sizeof(revData));
				closesocket(sClient);
				break;
			}

/**************是否断开检测***********************/
			if (ret == 0)
			{
				cout << "网络中断" << endl;
				memset(revData, 0, sizeof(revData));
				closesocket(sClient);
				break;
			}
			
/****************报头检测**********************/
			MBAPheadJuage dataJuage;
			SystemChange changes;
			if (!dataJuage.MBAPhead_Juage(revData, ret))
			{
				memset(revData, 0, sizeof(revData));
				continue;
			}

/*************异常码检测******************************/
			int Rlen = ret;
			DataJuage(revData, &Rlen);
			if (Rlen != ret)
			{
				send(sClient, (const char*)(&revData), Rlen, 0);
				memset(revData, 0, sizeof(revData));
				cout << "已发送异常码" << endl;
				continue;
			}

/**************线圈读写回复************************/
			int retlen = 12;
			if ((uint16_t)(revData[7] & 0xff) == 1 || (uint16_t)(revData[7] & 0xff) == 15)
			{
				uint8_t Coil[1024] = {};
				memcpy(Coil, revData, 12);

				Coilrw(Coil, &retlen, revData);
				//更改字节数
				Coil[4] = ((retlen - 6) >> 8) & 0xff;
				Coil[5] = (retlen - 6) & 0xff;
				//发送
				send(sClient, (const char*)(&Coil), retlen, 0);
				cout << "已发送线圈" << endl;
				//16进制打印
				for (int i = 0; i < retlen; i++)
				{
					uint8_t h[5] = {};
					changes.nToHexstr(Coil[i], h, 2);
					cout << h << " ";
				}
				memset(Coil, 0, sizeof(Coil));
				cout << endl;
			}

/**************寄存器读写回复*********************/
			else
			{
				uint8_t Register[1024] = {};
				memcpy(Register, revData, 12);

				Regist(Register, &retlen, revData);

				//更改字节数
				Register[4] = ((retlen - 6) >> 8) & 0xff;
				Register[5] = (retlen - 6) & 0xff;
				//发送
				send(sClient, (const char*)(&Register), retlen, 0);
				cout << "已发送寄存器" << endl;
				//打印
				for (int i = 0; i < retlen; i++)
				{
					uint8_t h[5] = {};
					changes.nToHexstr(Register[i], h, 2);
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
