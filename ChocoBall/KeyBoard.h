#pragma once
#include "DirectInput.h"

class CKeyBoard :
	public CDirectInput
{
public:
	CKeyBoard();
	~CKeyBoard();
	HRESULT CreateInput(HWND, LPDIRECTINPUT8)override;
	void Update()override;	// キー情報更新
	void ClearBuffer();
	/*
	*	左スティックの傾き量を正規化された-1.0～1.0で取得。　
	*	左：マイナス、右：プラス
	*/
	float GetStickL_XFloat() override
	{
		if (IsPressRight())
		{
			return 1.0f;
		}
		else if (IsPressLeft()){
			return -1.0f;
		}
		return 0.0f;
	}
	/*
	*	左スティックの傾き量を正規化された-1.0～1.0で取得。　
	*	下：マイナス、上：プラス
	*/
	float GetStickL_YFloat() override
	{
		if (IsPressUp())
		{
			return 1.0f;
		}
		else if (IsPressDown()){
			return -1.0f;
		}
		return 0.0f;
	}
	/*
	*	左スティックの傾き量　
	*	左：マイナス、右：プラス
	*/
	SHORT GetStickL_X()override
	{
		if (IsPressRight())
		{
			return static_cast<SHORT>(IsPressRight());
		}
		return static_cast<SHORT>(IsPressLeft());
	}
	/*
	*	左スティックの傾き量　
	*	下：マイナス、上：プラス
	*/
	SHORT GetStickL_Y()override
	{
		if (IsPressUp())
		{
			return static_cast<SHORT>(IsPressUp());
		}
		return static_cast<SHORT>(IsPressDown());
	}
	/*
	*	右スティックの傾き量　
	*	左：マイナス、右：プラス
	*/
	SHORT GetStickR_X()override
	{
		return 0;
	}
	/*
	*	右スティックの傾き量　
	*	下：マイナス、上：プラス
	*/
	SHORT GetStickR_Y()override
	{
		return 0;
	}

	/*
	*	左トリガーの深度
	*  0～255
	*/
	BYTE  GetTriggerL()override
	{
		return GetPrs(DIK_LCONTROL);
	}
	/*
	*	右トリガーの深度
	*  0～255
	*/
	BYTE  GetTriggerR()override
	{
		return GetPrs(DIK_RCONTROL);
	}

	bool GetConnected()override{ return true; };
	BYTE IsTriggerDecsion()override;// 決定ボタンが押されたら
	BYTE IsPressDecision()override;	// 決定ボタンが長押し
	BYTE IsTriggerCancel()override;	// キャンセル、戻るボタンが押されたら
	BYTE IsPressCancel()override;	// キャンセル、戻るボタンが長押し
	BYTE IsTriggerEscape()override;	// エスケープボタンが押されたら
	BYTE IsPressEscape()override;	// エスケープボタン長押し
	BYTE IsTriggerLeftShift()override;	// 左シフトキーが押されたら
	BYTE IsPressLeftShift()override;	// 左シフトキー長押し
	BYTE IsTriggerRightShift()override;	// 右シフトキーが押されたら
	BYTE IsPressRightShift()override;	// 右シフトキー長押し
	BYTE IsTriggerSpace()override;	// スペースキーが押されたら
	BYTE IsPressSpace()override;	// スペースキー長押し
	BYTE IsTriggerUp()override;		// ↑が押されたら
	BYTE IsPressUp()override;		// ↑が長押し
	BYTE IsTriggerDown()override;	// ↓が押されたら
	BYTE IsPressDown()override;		// ↓が長押し
	BYTE IsTriggerRight()override;	// →が押されたら
	BYTE IsPressRight()override;	// →が長押し
	BYTE IsTriggerLeft()override;	// ←が押されたら
	BYTE IsPressLeft()override;		// ←が長押し
private:
	//vector<LPDIRECTINPUTDEVICE8> m_DIKBDevices;	// DirectInputキーボードデバイス(複数接続されることを考慮して配列で管理する)
	//vector<DIDEVICEINSTANCE> m_InstPointer;	// DirectInputが自動生成するインスタンスへのポインターを格納する配列
	//int m_DeviceCount;						// デバイスをいくつ生成できたか
	BYTE GetPrs(const UINT KeyCode)const;	// キー判定(押しっぱなし)
	BYTE GetTrg(const UINT KeyCode)const;	// キー判定(押した瞬間)
	BYTE m_KBOldBuf[256];	// 一瞬前のキーボード情報
	BYTE m_KBNowBuf[256];	// 現在のキーボード情報
};
