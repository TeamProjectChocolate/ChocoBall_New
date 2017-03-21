#pragma once
#include "GameCamera.h"
#include "CourceDef.h"
#include "islntersect.h"

class CCourceCamera :
	public CGameCamera
{
public:
	CCourceCamera(){};
	~CCourceCamera(){};
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void CourceTurn(D3DXVECTOR3&,D3DXVECTOR3&,float,float);
	void ClearCamera();
	void SetStageID(STAGE_ID id){
		m_StageID = id;
	}
	void SetGameState(GAMEEND::ID id){
		m_GameState = id;
	}
	bool GetIsEnd(){
		return m_IsEnd;
	}
	void SetIsTarget(bool flg){
		m_IsTarget = flg;
	}
	void SetTargetViewAngle(float TV){
		m_TargetViewAngle = TV;
	}
private:
	CCourceDef m_courceDef;
	Cource::COURCE_BLOCK m_PrevCource;	// プレイヤーがワンフレーム前にいたブロック
	Cource::COURCE_BLOCK m_CurrentCource;	// プレイヤーが現在いるブロック
	bool m_CompCamera;			// カメラの補完処理	
	D3DXVECTOR3 m_NowPos;		// 現在のカメラの位置
	D3DXVECTOR3 m_cameraPosSpeed;	//カメラの移動速度。
	float m_NowViewAngle;		// 現在の画角。
	float m_TargetViewAngle;	// 目標画角。
	float m_CameraZoomSpeed;	// カメラのズーム速度。
	bool m_isFirst;
	STAGE_ID m_StageID;
	bool m_TurnFlg;
	GAMEEND::ID m_GameState;
	bool m_IsEnd;
	CIsIntersect m_Isintersect;
	bool m_IsTarget;
	D3DXVECTOR3 m_Dir;
	D3DXVECTOR3 m_TargetPos;
};

