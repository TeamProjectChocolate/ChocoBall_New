#include "stdafx.h"
#include "InputManager.h"


CInputManager* CInputManager::m_instance = nullptr;


HRESULT WINAPI CInputManager::DI_Init(){
	HRESULT ret = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_pInputObject, NULL);	// DirectInputのオブジェクト作成
	if (FAILED(ret)){
		MessageBox(0, TEXT("DirectInputオブジェクトの作成に失敗しました"), NULL, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CInputManager::CreateInput(HWND hWnd){
	CKeyBoard* Input;
	Input = new CKeyBoard;
	Input->CreateInput(hWnd, m_pInputObject);
	Add(Input);

	CGamePad* gamepad = new CGamePad;
	gamepad->Update();
	Add(gamepad);

}

void CInputManager::Add(CKeyBoard* Input){
	Input->SetType(INTERFACE_TYPE::KEYBOARD);
	m_Inputs.push_back(Input);
	SetCurrentInput(Input);
	m_SubInput = Input;
}

void CInputManager::Add(CGamePad* Input){
	Input->SetType(INTERFACE_TYPE::GAMEPAD);
	m_Inputs.push_back(Input);
	if (Input->GetConnected()){
		SetCurrentInput(Input);
	}
	else{
		SetCurrentInput(m_SubInput);
		m_SubInput = Input;
	}
}

void CInputManager::Update(){
	m_currentInput->Update();
	if (m_currentInput->GetType() == INTERFACE_TYPE::KEYBOARD){
		m_SubInput->Update();
		if (m_SubInput->GetConnected()){
			static_cast<CKeyBoard*>(m_currentInput)->ClearBuffer();
			CInterface* work = m_currentInput;
			SetCurrentInput(m_SubInput);
			m_SubInput = work;
			return;
		}
	}
	if (!m_currentInput->GetConnected()){
		CInterface* work = m_currentInput;
		SetCurrentInput(m_SubInput);
		m_SubInput = work;
	}
}

void CInputManager::ReleaseObject(){
	for (vector<CInterface*>::iterator itr = m_Inputs.begin(); itr == m_Inputs.end();){
		(*itr)->~CInterface();
		itr = m_Inputs.erase(itr);
	}
	SAFE_RELEASE(m_pInputObject);
}

void CInputManager::DeleteAll(){
	for (int idx = 0, size = m_Inputs.size(); idx < size; idx++){
		SAFE_DELETE(m_Inputs[idx]);
	}
	m_Inputs.clear();
}