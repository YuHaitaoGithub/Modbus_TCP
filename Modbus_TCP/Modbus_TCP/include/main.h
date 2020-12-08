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

//#define CoilFilename  "./Coil.ini"
#define CoilFilename  "E:\\Modbus agreement\\TCP_Slave\\Modbus_TCP\\Modbus_TCP\\Coil.ini"
#define CoilSection   "Coil"

//#define ResFilename   "./Register.ini"
#define ResFilename   "E:\\Modbus agreement\\TCP_Slave\\Modbus_TCP\\Modbus_TCP\\Register.ini"
#define ResSection    "register"


#pragma comment(lib,"ws2_32.lib")

using namespace std;


void Coilrw(uint8_t* in_num,int* retlen, uint8_t* Rdata);

void Regist(uint8_t* ret_num, int* retlenth, uint8_t* Reivedata);

void DataJuage(uint8_t* Rdata, int* Rlen);