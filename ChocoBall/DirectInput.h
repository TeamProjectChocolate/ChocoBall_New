#pragma once
#include "stdafx.h"
#include "Interface.h"

class CDirectInput:public CInterface{
public:
	CDirectInput(){
		m_Device = nullptr;
	};
	~CDirectInput(){
		SAFE_RELEASE(m_Device);
	}
	virtual HRESULT CreateInput(HWND, LPDIRECTINPUT8) = 0;
	HRESULT WINAPI CooperativeLevel(HWND, LPDIRECTINPUTDEVICE8);	// DirectInput排他制御設定処理
protected:
	LPDIRECTINPUTDEVICE8 m_Device;
};

//グローバル関数を定義してどこかにある実体を返す(ここでいうとDirectInputオブジェクトを返す)

