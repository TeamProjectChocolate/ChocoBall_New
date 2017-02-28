#pragma once

#include "stdafx.h"
class CTest{
	//SINGLETON_DECL(CTest);
private:										
	CTest(){}
	~CTest(){}
	static CTest* m_instance;
public:											
	static CTest*	GetInstance()
	{											
		return m_instance;						
	}											
	static CTest*	CreateInstance()
	{											
		if (m_instance == NULL){				
			m_instance = new CTest();
		}											
		return m_instance;						
	}

};