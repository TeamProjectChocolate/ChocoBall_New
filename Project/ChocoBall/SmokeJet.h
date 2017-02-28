#pragma once
#include "JetGimmick.h"
#include "Player.h"
#include "Vibration.h"
#include "CourceCamera.h"

class CSmokeJet :
	public CJetGimmick
{
public:
	CSmokeJet();
	~CSmokeJet();
	void Initialize()override;
	void Update()override;
	void SetName(LPCSTR name){
		strcpy(m_pFileName, name);
	}
};
