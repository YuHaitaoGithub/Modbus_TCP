#include"..\include\main.h"



int main()
{
	/*��ʼ����Ȧ�ͼĴ���*/
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



	//��ʼ��WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//�����׽���
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//��IP�Ͷ˿�
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(502);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)& sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}


	//��ʼ����
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}


	//ѭ����������
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	uint8_t revData[260];//255+6
	while (true)
	{
		printf("�ȴ�����...\n");
		sClient = accept(slisten, (SOCKADDR*)& remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		u_long mode = 0;
		ioctlsocket(sClient, FIONBIO, &mode);
		
/***********�ѵ�ǰ�׽��ֶ�ȡ��******************/
		for (;;)
		{
			if (sClient == NULL)
			{
				closesocket(sClient);
				break;
			}
		
			int ret = recv(sClient, (char*)revData, 261, 0);

/********************���ܴ����ж�******************/
			if (ret == SOCKET_ERROR)
			{
				cout << "�����ж�" << endl;
				memset(revData, 0, sizeof(revData));
				closesocket(sClient);
				break;
			}

/**************�Ƿ�Ͽ����***********************/
			if (ret == 0)
			{
				cout << "�����ж�" << endl;
				memset(revData, 0, sizeof(revData));
				closesocket(sClient);
				break;
			}
			
/****************��ͷ���**********************/
			MBAPheadJuage dataJuage;
			SystemChange changes;
			if (!dataJuage.MBAPhead_Juage(revData, ret))
			{
				memset(revData, 0, sizeof(revData));
				continue;
			}

/*************�쳣����******************************/
			int Rlen = ret;
			DataJuage(revData, &Rlen);
			if (Rlen != ret)
			{
				send(sClient, (const char*)(&revData), Rlen, 0);
				memset(revData, 0, sizeof(revData));
				cout << "�ѷ����쳣��" << endl;
				continue;
			}

/**************��Ȧ��д�ظ�************************/
			int retlen = 12;
			if ((uint16_t)(revData[7] & 0xff) == 1 || (uint16_t)(revData[7] & 0xff) == 15)
			{
				uint8_t Coil[1024] = {};
				memcpy(Coil, revData, 12);

				Coilrw(Coil, &retlen, revData);
				//�����ֽ���
				Coil[4] = ((retlen - 6) >> 8) & 0xff;
				Coil[5] = (retlen - 6) & 0xff;
				//����
				send(sClient, (const char*)(&Coil), retlen, 0);
				cout << "�ѷ�����Ȧ" << endl;
				//16���ƴ�ӡ
				for (int i = 0; i < retlen; i++)
				{
					uint8_t h[5] = {};
					changes.nToHexstr(Coil[i], h, 2);
					cout << h << " ";
				}
				memset(Coil, 0, sizeof(Coil));
				cout << endl;
			}

/**************�Ĵ�����д�ظ�*********************/
			else
			{
				uint8_t Register[1024] = {};
				memcpy(Register, revData, 12);

				Regist(Register, &retlen, revData);

				//�����ֽ���
				Register[4] = ((retlen - 6) >> 8) & 0xff;
				Register[5] = (retlen - 6) & 0xff;
				//����
				send(sClient, (const char*)(&Register), retlen, 0);
				cout << "�ѷ��ͼĴ���" << endl;
				//��ӡ
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
