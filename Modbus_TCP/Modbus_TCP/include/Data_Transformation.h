#ifndef _DATA_TRANSFORMATION_H
#define _DATA_TRANSFORMATION_H

#include <stdio.h>
#include <string.h>
#include"stdint.h"
#include "malloc.h"
#include <iostream>

#define AddRess 0xff


using namespace std;

class SystemChange{
public:
	SystemChange();
	~SystemChange();

	/*�ַ���תδ����*/
	uint16_t SystemChange::ChangeNum(uint8_t* str);

	/*�ַ�תʮ��������ʾ*/
	void nToHexstr(uint8_t n, unsigned char * hexstr, uint8_t strlen);

	/*���ܳ����ж�*/
	uint16_t ReceiveLenth(uint8_t* data);

	/*MBAPͷ���ж�*/
	bool MBAPhead_Juage(uint8_t* Rdata, int Rlen);


	/*�쳣������*/
	void DataEorry(uint8_t* &Rdata, int* Rlen, uint16_t code, uint16_t function);
};

#endif