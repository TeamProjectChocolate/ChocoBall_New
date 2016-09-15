#pragma once
#include "stdafx.h"

class CCalculation{
public:
	CCalculation();
	~CCalculation();
	virtual void Initialize() = 0;
	virtual short Update(short,short) = 0;
	short AddNum(short num1, short num2){ return num1 + num2; };
	short SubNum(short num1, short num2){ return num1 - num2; };
	short MultiNum(short num1, short num2){ return num1 * num2; };
	short DivNum(short num1, short num2){ return num1 / num2; };
};