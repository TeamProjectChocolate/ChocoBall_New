#pragma once
#include "GameObject.h"
#include "DirectInput.h"

class CTestObject2D :
	public CGameObject
{
public:
	CTestObject2D();
	~CTestObject2D();
	void Initialize()override;
	void Update()override;
	void Draw()override;
private:
	CInterface* m_Input = nullptr;
	float m_angle;
};

