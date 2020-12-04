#pragma once
#define _CRT_SECURE_NO_WARNINGS 

#include "stdint.h"
#include<iostream>
#include<string>
#include<vector>
#include "Data_Transformation.h"
#include <winsock2.h>
#include <time.h>
#include <math.h>

#define CoilFilename  "./Coil.ini"
#define CoilSection   "Coil"

#define ResFilename   "./Register.ini"
#define ResSection    "register"


#pragma comment(lib,"ws2_32.lib")

using namespace std;


void Coilrw(uint8_t* in_num,int* retlen, uint8_t* Rdata);

void Regist(uint8_t* ret_num, int* retlenth, uint8_t* Reivedata);