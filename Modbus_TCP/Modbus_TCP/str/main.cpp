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
			cout << "�Ƿ����ݵ�ַ,�����쳣��02" << endl;
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
					cout << "�Ƿ�����ֵ,�����쳣��03" << endl;
					code = 3;
					numJuage.DataEorry(Rdata, Rlen, code, function);
					return;
				}
				return;
			}
			if (number > 125 || number < 1 || number + address > 125)
			{
				cout << "�Ƿ�����ֵ,�����쳣��03" << endl;
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
					cout << "�Ƿ�����ֵ,�����쳣��03" << endl;
					code = 3;
					numJuage.DataEorry(Rdata, Rlen, code, function);
					return;
				}
				return;
			}
			if (number > 123 || number < 1 || number + address > 123 || Bytenum != (*Rlen) - 13)
			{
				cout << "�Ƿ�����ֵ,�����쳣��03" << endl;
				code = 3;
				numJuage.DataEorry(Rdata, Rlen, code, function);
			}
			return;
		}
	}
	else {
		cout << "�Ƿ�������,�����쳣��01" << endl;
		code = 1;
		numJuage.DataEorry(Rdata, Rlen, code, function);
		return;
	}
}



int main(int argc, char* argv[])
{
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
	uint8_t revData[256];
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

		int ret = recv(sClient, (char*)revData, 256, 0);
		if (ret == SOCKET_ERROR)
		{
			cout << "���մ���" << endl;
			memset(revData, 0, sizeof(revData));
			continue;
		}

		if (ret == 0)
		{
			cout << "�����ж�" << endl;
			memset(revData, 0, sizeof(revData));
			continue;
		}

		if (ret < 12)
		{
			cout << "���ݳ��Ȳ��Ϸ�";
			memset(revData, 0, sizeof(revData));
			continue;
		}

		SystemChange dataJuage;
		if (!dataJuage.MBAPhead_Juage(revData, ret))
		{
			memset(revData, 0, sizeof(revData));
			cout << "MBAPͷ����ȷ"<< endl;
			continue;
		}
		int Rlen = ret;
		DataJuage(revData, &Rlen);
		if (Rlen != ret)
		{
			send(sClient, (const char*)(&revData), Rlen, 0);
			cout << "�ѷ����쳣��" << endl;
		}
		closesocket(sClient);
	}
	closesocket(slisten);
	WSACleanup();
	return 0;
}
