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

	uint16_t SystemChange::ChangeNum(uint8_t* str);

	void nToHexstr(uint8_t n, unsigned char * hexstr, uint8_t strlen);

	uint16_t ReceiveLenth(uint8_t* data);

	bool MBAPhead_Juage(uint8_t* Rdata, int Rlen);

	void DataEorry(uint8_t* &Rdata, int* Rlen, uint16_t code, uint16_t function);
};

#endif