#pragma once
#define _CRT_SECURE_NO_WARNINGS 

#include "stdint.h"
#include<iostream>
#include<string>
#include<vector>
#include "Data_Transformation.h"
#include"MBAPjuage.h"
#include <winsock2.h>
#include <time.h>
#include <math.h>


#define CoilNum_Max   2500
#define RestNum_Max   250

#define Portnum       502

#pragma comment(lib,"ws2_32.lib")

using namespace std;


void Coilrw(uint8_t* in_num, int* retlen, uint8_t* Rdata, uint8_t *lpcoil);

void Regist(uint8_t* ret_num, int* retlenth, uint8_t* Reivedata, uint8_t *lprest);

void DataJuage(uint8_t* Rdata, int* Rlen);