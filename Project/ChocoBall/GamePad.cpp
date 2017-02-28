#include "stdafx.h"
#include "GamePad.h"


CGamePad::CGamePad()
{
	m_bConnected = false;
	m_GPNowBuf = false;
	m_GPOldBuf = false;
}

CGamePad::~CGamePad()
{
}

void CGamePad::Update(){
	if (XInputGetState(0, &m_state) == ERROR_SUCCESS)
	{
		m_bConnected = true;
		// 1フレーム前のバッファと現在のバッファを更新
		ZeroMemory(&m_GPOldBuf, sizeof(m_GPOldBuf));
		memcpy(&m_GPOldBuf, &m_GPNowBuf, sizeof(m_GPNowBuf));
		m_GPNowBuf = m_state.Gamepad.wButtons;
	}
	else
	{
		m_bConnected = false;
	}
}

void CGamePad::ClearBuffer(){
	ZeroMemory(&m_GPNowBuf, sizeof(m_GPNowBuf));
	memcpy(&m_GPOldBuf, &m_GPNowBuf, sizeof(m_GPNowBuf));
}


bool CGamePad::GetTrg(const int& Buttons){
	return (!(m_GPOldBuf & Buttons) && (m_GPNowBuf & Buttons));
}

bool CGamePad::GetPrs(const int& Buttons){
	return (m_GPNowBuf & Buttons);
}


BYTE CGamePad::IsTriggerDecsion(){ return GetTrg(GAMEPAD_A); }
BYTE CGamePad::IsPressDecision(){ return GetPrs(GAMEPAD_A); }
BYTE CGamePad::IsTriggerCancel(){ return GetTrg(GAMEPAD_B); }
BYTE CGamePad::IsPressCancel(){ return GetPrs(GAMEPAD_B); }
BYTE CGamePad::IsTriggerEscape(){ return GetTrg(GAMEPAD_START); }
BYTE CGamePad::IsPressEscape(){ return GetPrs(GAMEPAD_START); }
BYTE CGamePad::IsTriggerLeftShift(){ return GetTrg(GAMEPAD_LEFT_SHOULDER); }
BYTE CGamePad::IsPressLeftShift(){ return GetPrs(GAMEPAD_LEFT_SHOULDER); }
BYTE CGamePad::IsTriggerRightShift(){ return GetTrg(GAMEPAD_RIGHT_SHOULDER); }
BYTE CGamePad::IsPressRightShift(){ return GetPrs(GAMEPAD_RIGHT_SHOULDER); }
BYTE CGamePad::IsTriggerSpace(){ return GetTrg(GAMEPAD_A); }
BYTE CGamePad::IsPressSpace(){ return GetPrs(GAMEPAD_A); }
BYTE CGamePad::IsTriggerUp(){ return GetTrg(GAMEPAD_DPAD_UP); }
BYTE CGamePad::IsPressUp(){ return GetPrs(GAMEPAD_DPAD_UP); }
BYTE CGamePad::IsTriggerDown(){ return GetTrg(GAMEPAD_DPAD_DOWN); }
BYTE CGamePad::IsPressDown(){ return GetPrs(GAMEPAD_DPAD_DOWN); }
BYTE CGamePad::IsTriggerRight(){ return GetTrg(GAMEPAD_DPAD_RIGHT); }
BYTE CGamePad::IsPressRight(){ return GetPrs(GAMEPAD_DPAD_RIGHT); }
BYTE CGamePad::IsTriggerLeft(){ return GetTrg(GAMEPAD_DPAD_LEFT); }
BYTE CGamePad::IsPressLeft(){ return GetPrs(GAMEPAD_DPAD_LEFT); }