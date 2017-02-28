#pragma once
#include "JetGimmick.h"
#include "Player.h"
#include "Vibration.h"
#include "CourceCamera.h"


class CFireJet :
	public CJetGimmick
{
public:
	CFireJet();
	~CFireJet();
	void Initialize()override;
	void Update()override;
	bool IsCollision(D3DXVECTOR3,float);
	void SetName(LPCSTR name){
		strcpy(m_pFileName, name);
	}
};

