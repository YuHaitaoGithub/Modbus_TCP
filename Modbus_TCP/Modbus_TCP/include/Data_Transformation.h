#ifndef _DATA_TRANSFORMATION_H
#define _DATA_TRANSFORMATION_H

#include <stdio.h>
#include <string.h>
#include"stdint.h"
#include "malloc.h"
#include <iostream>


using namespace std;

class SystemChange{
public:
	SystemChange();
	~SystemChange();

	/*�ַ�תʮ��������ʾ*/
	void nToHexstr(uint8_t n, unsigned char * hexstr, uint8_t strlen);

};

#endif