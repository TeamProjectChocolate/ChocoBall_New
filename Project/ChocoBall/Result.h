#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "ClearText.h"
#include "GameOver.h"

class CResult:public CGameObject
{
public:
	CResult();
	~CResult();
	void Initialize()override;
	void Update()override;
	void Draw()override;
private:
	CClearText m_Text;
	CGameOver  m_Over;
};