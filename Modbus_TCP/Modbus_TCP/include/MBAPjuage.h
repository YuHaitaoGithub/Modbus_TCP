#ifndef _MBAPJUAGE_H_
#define _MBAPJUAGE_H_


#include <stdio.h>
#include <string.h>
#include"stdint.h"
#include "malloc.h"
#include <iostream>

#define AddRess 0xFF


using namespace std;

class MBAPheadJuage{
public:
	MBAPheadJuage();
	~MBAPheadJuage();

	/*MBAP头的判断*/
	bool MBAPhead_Juage(uint8_t* Rdata, int Rlen);

	/*异常码生成*/
	void DataEorry(uint8_t* &Rdata, int* Rlen, uint16_t code, uint16_t function);

	uint16_t ChangeNum(uint8_t* str);
};
#endif