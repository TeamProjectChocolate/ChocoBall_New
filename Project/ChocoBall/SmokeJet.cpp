#include "stdafx.h"
#include "SmokeJet.h"
#include "CourceDef.h"


CSmokeJet::CSmokeJet()
{
}


CSmokeJet::~CSmokeJet()
{
}

void CSmokeJet::Initialize(){
	m_IntervalTime = 6.0f;
	m_JetTime = 6.0f;
	m_ParticleType = PARTICLE_TYPE::SOMKEGIMMICK;
	strcpy(m_SoundName, "Flame");
	strcpy(m_SoundEndName, "Flameend");

	CJetGimmick::Initialize();
}

void CSmokeJet::Update(){
	CJetGimmick::Update();
}