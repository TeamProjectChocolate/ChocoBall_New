#pragma once
#include "stdafx.h"
#include "KeyBoard.h"
#include "GamePad.h"

class CInputManager
{
	SINGLETON_DECL(CInputManager);
public:
	//DirectInputオブジェクト生成関数
	//引き数: なし
	//返り値: HRESULT型 エラーコードを返却
	//呼び出し例: SINSTANCE(CInputManager)->DI_Init();
	//※プログラム中で必ず最初に一回だけ呼び出してください
	HRESULT WINAPI DI_Init(); 

	//キーボードのDirectInputデバイスの生成関数
	//引き数: HWND型 ウィンドウハンドル
	//返り値: なし
	//呼び出し例: SINSTANCE(CInputManager)->CreateKeyBoard(g_hWnd);
	//※プログラム中で必ず最初に一回だけ呼び出してください
	void CreateInput(HWND hWnd);

	void Add(CKeyBoard*);
	void Add(CGamePad*);

	void SetCurrentInput(CInterface* Input){
		m_currentInput = Input;
	}

	// 現在のインターフェースを取得する関数
	CInterface* GetCurrentInput(){
		if (m_currentInput->GetType() == INTERFACE_TYPE::GAMEPAD){
			return static_cast<CGamePad*>(m_currentInput);
		}
		return static_cast<CKeyBoard*>(m_currentInput);
	}

	// インターフェースが切り替わっているか確認する関数
	// 切り替わっていれば自動で切り替える
	// 引き数: CInterface**型 インターフェースのダブルポインタ
	// 呼び出し例: SINSTANCE(CInputManager)->IsInputChanged(&m_Input);
	// ※入力が必要なクラスで必ず毎フレーム呼び出してください
	void IsInputChanged(CInterface** Input){
		if (*Input == m_currentInput){
			return;
		}
		else{
			*Input = GetCurrentInput();
			return;
		}
	}

	void Update();
	void ReleaseObject();
private:
	LPDIRECTINPUT8 m_pInputObject = nullptr;	// DirectInputオブジェクト
	vector<CInterface*> m_Inputs;	// キーボードが複数あれば複数格納
	CInterface* m_currentInput = nullptr;	// メインのインターフェース
	CInterface* m_SubInput = nullptr;	// 切り替え用インターフェース(メインがキーボードの時はゲームパッド、メインがゲームパッドのときはキーボード)
	//vectorに登録された要素をすべて削除する関数
	//※デストラクタにて呼び出される
	void DeleteAll();
};

