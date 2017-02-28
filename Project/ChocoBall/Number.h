#pragma once
#include "GameObject.h"
#include "stdafx.h"
#include "Number.h"

#define NUMBER_NUM 10

class CNumber :public CGameObject{
public:
	CNumber();
	~CNumber();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void SetValue(const int value){ m_value = value; };		// 外部から受け取った値をm_valueに格納
	void AddValue(const int value){ m_value += value; };		// 外部から受け取った値をm_valueに加算
private:
	int m_value;	// 数値
};