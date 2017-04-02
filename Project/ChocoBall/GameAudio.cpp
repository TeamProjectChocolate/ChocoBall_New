#include "stdafx.h"
#include "GameAudio.h"


CGameAudio::CGameAudio()
{
}


CGameAudio::~CGameAudio()
{
}

void CGameAudio::Initialize() {
}

void CGameAudio::Initialize(const char* EngineName, const char* WBName, const char* SBName) {
	m_pAudio.reset(new CAudio);
	m_pAudio->Initialize(EngineName, WBName, SBName);
}

void CGameAudio::Update() {
	m_pAudio->Run();
}

void CGameAudio::Draw() {
}
