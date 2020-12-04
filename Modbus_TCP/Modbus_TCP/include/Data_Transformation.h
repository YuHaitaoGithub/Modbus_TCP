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

	/*字符串转未整数*/
	uint16_t SystemChange::ChangeNum(uint8_t* str);

	/*字符转十六进制显示*/
	void nToHexstr(uint8_t n, unsigned char * hexstr, uint8_t strlen);

	/*接受长度判断*/
	uint16_t ReceiveLenth(uint8_t* data);

	/*MBAP头的判断*/
	bool MBAPhead_Juage(uint8_t* Rdata, int Rlen);


	/*异常码生成*/
	void DataEorry(uint8_t* &Rdata, int* Rlen, uint16_t code, uint16_t function);
};

#endif