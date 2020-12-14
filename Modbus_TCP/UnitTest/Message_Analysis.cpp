#include"stdafx.h"


#define filename  "..\\Modbus_TCP\\test\\TCPTest.ini"
#define MAX_LINE 1024
#define receive  "RequestMessage"
#define receveBytenum  "RequestByteNum"
#define output  "Output"

int ReceveBytenum = 0;


void DataRead(char *section, unsigned char *lpRecevebuffer, unsigned char *outbuffer)
{
	MBAPheadJuage data;
	/*请求数据读取处理*/
	char sendbuf[1024] = {};
	char buf[5] = {};
	ReceveBytenum = GetPrivateProfileIntA(section, receveBytenum, -1, filename);
	GetPrivateProfileStringA(section, receive, NULL, sendbuf, 1024, filename);

	char *p = strtok(sendbuf, " ");
	memcpy(buf, p, 2);
	int k = data.ChangeNum((unsigned char*)buf);
	lpRecevebuffer[0] = k & 0xff;
	p = NULL;
	memset(buf, 0, 5);
	for (int i = 1; i < ReceveBytenum; i++)
	{
		p = strtok(NULL, " ");
		if (p == NULL)break;
		memcpy(buf, p, 2);
		k = data.ChangeNum((unsigned char*)buf);
		lpRecevebuffer[i] = k & 0xff;
		p = NULL;
		memset(buf, 0, 5);
	}

	char outbuf[1024] = {};
	GetPrivateProfileStringA(section, output, NULL, outbuf, 1024, filename);
	if (outbuf[0] == '\0')return;
	p = strtok(outbuf, " ");
	memcpy(buf, p, 2);
	k = data.ChangeNum((unsigned char*)buf);
	outbuffer[0] = k & 0xff;
	p = NULL;
	memset(buf, 0, 5);
	int v = 1;
	while (1)
	{
		p = strtok(NULL, " ");
		if (p == NULL)
			break;
		memcpy(buf, p, 2);
		k = data.ChangeNum((unsigned char*)buf);
		outbuffer[v++] = k & 0xff;
		p = NULL;
		memset(buf, 0, 5);
	}
	return;
}


void DataTest()
{
	char section1[MAX_LINE] = { '\0' };//section1--该文件下所有节名保存数组
	int a = 0;
	GetPrivateProfileSectionNamesA(section1, MAX_LINE, filename);
	char section[MAX_LINE] = { '\0' };//保存单个节名数组
	int time = 0;
	while ((section1[a] != '\0') || (section1[a + 1] != '\0'))
	{
		for (int k = a; section1[k] != '\0'; k++)
			section[time++] = section1[k];

		MBAPheadJuage data;
		unsigned char Recevebuffer[1024] = {};
		unsigned char outbuffer[1024] = {};
		DataRead(section, Recevebuffer,outbuffer);

		if (!data.MBAPhead_Juage(Recevebuffer, ReceveBytenum))
		{
			Assert::AreEqual((unsigned char)0, outbuffer[0]);
			Logger::WriteMessage("MBAP or other erroy");
		}
		else
		{
			int Rlen = ReceveBytenum;
			DataJuage(Recevebuffer, &Rlen);
			if (Rlen != ReceveBytenum)
			{
				for (int i = 0; i < Rlen; i++)
					Assert::AreEqual(Recevebuffer[i], outbuffer[i]);
			}
			else
			{
				int retlen = 12;
				if ((uint16_t)(Recevebuffer[7] & 0xff) == 1 || (uint16_t)(Recevebuffer[7] & 0xff) == 15)
				{
					uint8_t Coil[1024] = {};
					memcpy(Coil, Recevebuffer, 12);

					Coilrw(Coil, &retlen, Recevebuffer);
					Coil[4] = ((retlen - 6) >> 8) & 0xff;
					Coil[5] = (retlen - 6) & 0xff;
					//发送
					for (int i = 0; i < retlen; i++)
						Assert::AreEqual(Coil[i], outbuffer[i]);
					memset(Coil, 0, sizeof(Coil));
				}
				else
				{
					uint8_t Register[1024] = {};
					memcpy(Register, Recevebuffer, 12);
					Regist(Register, &retlen, Recevebuffer);

					Register[4] = ((retlen - 6) >> 8) & 0xff;
					Register[5] = (retlen - 6) & 0xff;
					//发送
					for (int i = 0; i < retlen; i++)
						Assert::AreEqual(Register[i], outbuffer[i]);
					memset(Register, 0, sizeof(Register));
				}

			}

		}
		a = a + time + 1;
		time = 0;
		memset(section, 0, sizeof(section));
	}
	return;
}