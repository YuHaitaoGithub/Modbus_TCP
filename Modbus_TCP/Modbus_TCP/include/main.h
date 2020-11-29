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

#pragma comment(lib,"ws2_32.lib")

using namespace std;


void Registe(uint8_t* in_num, int* ret, uint8_t* Rdata);