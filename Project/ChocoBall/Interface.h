#pragma once

#include "stdafx.h"

typedef enum INTERFACE_TYPE{ KEYBOARD = 0, GAMEPAD,INTERFACE_NONE = 100 };

class CInterface
{
public:
	CInterface();
	~CInterface();
	virtual void Update() = 0;
	INTERFACE_TYPE GetType(){
		return m_InputType;
	}
	void SetType(INTERFACE_TYPE type){
		m_InputType = type;
	}
	/*
	*	左スティックの傾き量を正規化された-1.0～1.0で取得。　
	*	左：マイナス、右：プラス
	*/
	virtual float GetStickL_XFloat() = 0;
	/*
	*	左スティックの傾き量を正規化された-1.0～1.0で取得。　
	*	下：マイナス、上：プラス
	*/
	virtual float GetStickL_YFloat() = 0;
	/*
	*	左スティックの傾き量　
	*	左：マイナス、右：プラス
	*/
	virtual SHORT GetStickL_X() = 0;
	/*
	*	左スティックの傾き量　
	*	下：マイナス、上：プラス
	*/
	virtual SHORT GetStickL_Y() = 0;
	/*
	*	右スティックの傾き量　
	*	左：マイナス、右：プラス
	*/
	virtual SHORT GetStickR_X() = 0;
	/*
	*	右スティックの傾き量　
	*	下：マイナス、上：プラス
	*/
	virtual SHORT GetStickR_Y() = 0;
	/*
	*	左トリガーの深度
	*  0～255
	*/
	virtual BYTE  GetTriggerL() = 0;
	/*
	*	右トリガーの深度
	*  0～255
	*/
	virtual BYTE  GetTriggerR() = 0;

	virtual bool GetConnected() = 0;

	virtual BYTE IsTriggerDecsion() = 0;// 決定ボタンが押されたら
	virtual BYTE IsPressDecision() = 0;	// 決定ボタンが長押し
	virtual BYTE IsTriggerCancel() = 0;	// キャンセル、戻るボタンが押されたら
	virtual BYTE IsPressCancel() = 0;	// キャンセル、戻るボタンが長押し
	virtual BYTE IsTriggerEscape() = 0;	// エスケープボタンが押されたら
	virtual BYTE IsPressEscape() = 0;	// エスケープボタン長押し
	virtual BYTE IsTriggerLeftShift() = 0;	// 左シフトキーが押されたら
	virtual BYTE IsPressLeftShift() = 0;	// 左シフトキー長押し
	virtual BYTE IsTriggerRightShift() = 0;	// 右シフトキーが押されたら
	virtual BYTE IsPressRightShift() = 0;	// 右シフトキー長押し
	virtual BYTE IsTriggerSpace() = 0;	// スペースキーが押されたら
	virtual BYTE IsPressSpace() = 0;	// スペースキー長押し
	virtual BYTE IsTriggerUp() = 0;		// ↑が押されたら
	virtual BYTE IsPressUp() = 0;		// ↑が長押し
	virtual BYTE IsTriggerDown() = 0;	// ↓が押されたら
	virtual BYTE IsPressDown() = 0;		// ↓が長押し
	virtual BYTE IsTriggerRight() = 0;	// →が押されたら
	virtual BYTE IsPressRight() = 0;	// →が長押し
	virtual BYTE IsTriggerLeft() = 0;	// ←が押されたら
	virtual BYTE IsPressLeft() = 0;		// ←が長押し
private:
	INTERFACE_TYPE m_InputType;
};

