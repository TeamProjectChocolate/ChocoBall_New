#include "stdafx.h"
#include "Camera.h"
#include "GraphicsDevice.h"
#include "Effect.h"
#include "BulletPhysics.h"

CCamera::CCamera()
{
}


CCamera::~CCamera()
{
}

void CCamera::Initialize(){
	m_position.x = 0.0f;
	m_position.y = 1.2f;
	m_position.z = -20.0f;
	m_target.x = 0.0f;
	m_target.y = 0.1f;
	m_target.z = 0.0f;
	m_up.x = 0.0f;
	m_up.y = 1.0f;
	m_up.z = 0.0f;
	m_ViewAngle = D3DXToRadian(45);		// 45度をラジアンに変換して格納
	m_aspect = 1.0f / 1.0f;				// アスペクト比に計算
	m_Near = 1.0f;						// どこから
	m_Far = 1000.0f;					// どこまで描画するか
	m_updateType = enUpdateTypeTarget;
	m_Axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

void CCamera::Update(){
	// 射影マトリックスの設定
	D3DXMatrixIdentity(&m_Proj);			// 行列初期化
	D3DXMatrixPerspectiveFovLH(
		&m_Proj,							// 射影マトリックス
		m_ViewAngle,						// 画角
		m_aspect,							// アスペクト比
		m_Near,								// Nearプレーン
		m_Far);								// Farプレーン

	// ビュートランスフォーム(視点座標変換)
	D3DXMatrixIdentity(&m_View);									// 行列初期化
	D3DXVECTOR3 vEye(m_position.x, m_position.y, m_position.z);		// カメラ位置

	if (m_updateType == enUpdateTypeTarget){
		m_direction = m_target - m_position;
		D3DXVec3Normalize(&m_direction,&m_direction);
		float t = fabsf(D3DXVec3Dot(&m_direction, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)));
		if (fabsf((t - 1.0f)) < 0.00001f) {
			//ライトの方向がほぼY軸と並行。
			m_up = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		}
		else {
			m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
		D3DXMatrixLookAtLH(&m_View, &vEye, &m_target, &m_up);			// ビューマトリックス設定
	}
	else if (m_updateType == enUpdateTypeDirection){
		float t = fabsf(D3DXVec3Dot(&m_direction, &m_Axis));
		if (fabsf((t - 1.0f)) < 0.00001f) {
			//ライトの方向がほぼY軸と並行。
			m_up = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		}
		else {
			m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
		D3DXVECTOR3 target = vEye + m_direction;
		D3DXMatrixLookAtLH(&m_View, &vEye, &target, &m_up);
	}
	// カメラの回転行列の算出
	D3DXMatrixIdentity(&m_Rota);
	D3DXMatrixInverse(&m_Rota, nullptr, &m_View);
	m_Rota.m[3][0] = 0.0f;
	m_Rota.m[3][1] = 0.0f;
	m_Rota.m[3][2] = 0.0f;
	m_Rota.m[3][3] = 1.0f;

	D3DXMatrixIdentity(&m_RotaInv);
	D3DXMatrixInverse(&m_RotaInv, nullptr, &m_Rota);
}

void CCamera::SetCamera(LPD3DXEFFECT effect){
	effect->SetMatrix("Proj"/*エフェクトファイル内の変数名*/, &m_Proj/*設定したい行列へのポインタ*/);
	effect->SetMatrix("View"/*エフェクトファイル内の変数名*/, &m_View/*設定したい行列へのポインタ*/);
	effect->SetMatrix("g_CameraRotaInverse", &m_RotaInv);
}

void CCamera::SetFarNear(LPD3DXEFFECT effect){
	effect->SetVector("g_FarNear", reinterpret_cast<D3DXVECTOR4*>(&(D3DXVECTOR2(m_Far, m_Near))));
}