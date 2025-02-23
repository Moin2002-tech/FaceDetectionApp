#pragma once

/*
* MatReader.h
* Created on : 22/ 11 / 2024
* Author : Moin Shaikh
*/

#include<string>
#include"matio.h"

class MatlabReader
{
	MatlabReader(void);
	MatlabReader(const std::string);
	~MatlabReader(void);

	int getKey(const char*, char*);
	int getInt(const char*, int*);

private:
	mat_t* Matfile;

};