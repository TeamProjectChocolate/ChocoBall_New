#pragma once

#include "stdafx.h"
#include "Interface.h"


#define GAMEPAD(Class) Class::getGameInstance()

#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )

	//各ボタン
#define GAMEPAD_DPAD_UP          0x0001
#define GAMEPAD_DPAD_DOWN        0x0002
#define GAMEPAD_DPAD_LEFT        0x0004
#define GAMEPAD_DPAD_RIGHT       0x0008
#define GAMEPAD_START            0x0010
#define GAMEPAD_BACK             0x0020
#define GAMEPAD_LEFT_THUMB       0x0040
#define GAMEPAD_RIGHT_THUMB      0x0080
#define GAMEPAD_LEFT_SHOULDER    0x0100
#define GAMEPAD_RIGHT_SHOULDER   0x0200
#define GAMEPAD_A                0x1000
#define GAMEPAD_B                0x2000
#define GAMEPAD_X                0x4000
#define GAMEPAD_Y                0x8000


class CGamePad:public CInterface
{
public:
	CGamePad();
	~CGamePad();
	/*
	*	コントローラーが接続されているか判定
	*	ゲームのアップデートで必ず呼ぶ
	*/
	void Update()override;

	// バッファの削除
	void ClearBuffer();
	/*
	*	左スティックの傾き量を正規化された-1.0～1.0で取得。　
	*	左：マイナス、右：プラス
	*/
	float GetStickL_XFloat() override
	{
		return (float)GetStickL_X() / 0x7FFF;
	}
	/*
	*	左スティックの傾き量を正規化された-1.0～1.0で取得。　
	*	下：マイナス、上：プラス
	*/
	float GetStickL_YFloat() override
	{
		return (float)GetStickL_Y() / 0x7FFF;
	}

	/*
	*	左スティックの傾き量　
	*	左：マイナス、右：プラス
	*/
	SHORT GetStickL_X()override
	{
		if ((m_state.Gamepad.sThumbLX < INPUT_DEADZONE &&
			m_state.Gamepad.sThumbLX > -INPUT_DEADZONE))
		{
			m_state.Gamepad.sThumbLX = 0;
		}
		return m_state.Gamepad.sThumbLX;
	}
	/*
	*	左スティックの傾き量　
	*	下：マイナス、上：プラス
	*/
	SHORT GetStickL_Y()override
	{
		if ((m_state.Gamepad.sThumbLY < INPUT_DEADZONE &&
			m_state.Gamepad.sThumbLY > -INPUT_DEADZONE))
		{
			m_state.Gamepad.sThumbLY = 0;
		}
		return m_state.Gamepad.sThumbLY;
	}
	/*
	*	右スティックの傾き量　
	*	左：マイナス、右：プラス
	*/
	SHORT GetStickR_X()override
	{
		if ((m_state.Gamepad.sThumbRX < INPUT_DEADZONE &&
			m_state.Gamepad.sThumbRX > -INPUT_DEADZONE))
		{
			m_state.Gamepad.sThumbRX = 0;
		}
		return m_state.Gamepad.sThumbRX;
	}
	/*
	*	右スティックの傾き量　
	*	下：マイナス、上：プラス
	*/
	SHORT GetStickR_Y()override
	{
		if ((m_state.Gamepad.sThumbRY < INPUT_DEADZONE &&
			m_state.Gamepad.sThumbRY > -INPUT_DEADZONE))
		{
			m_state.Gamepad.sThumbRY = 0;
		}
		return m_state.Gamepad.sThumbRY;
	}

	/*
	*	左トリガーの深度
	*  0～255
	*/
	BYTE  GetTriggerL()override
	{
		return m_state.Gamepad.bLeftTrigger;
	}
	/*
	*	右トリガーの深度
	*  0～255
	*/
	BYTE  GetTriggerR()override
	{
		return m_state.Gamepad.bRightTrigger;
	}
	/*
	*	コントローラーがさされているか
	*/
	bool GetConnected()override
	{
		return m_bConnected;
	}

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
	XINPUT_STATE m_state;	//状態
	bool m_bConnected;	//コントローラー生存
	WORD m_GPNowBuf;
	WORD m_GPOldBuf;
	// ボタンが長押しされているか判定
	bool GetPrs(const int& Buttons);
	// ボタンが押されたか判定
	bool GetTrg(const int& Buttons);
};

