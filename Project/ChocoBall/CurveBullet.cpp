#include "stdafx.h"
#include "CurveBullet.h"


CCurveBullet::CCurveBullet()
{
}


CCurveBullet::~CCurveBullet()
{
}

void CCurveBullet::Initialize(const D3DXVECTOR3& ShotPos, D3DXVECTOR3* TargetPos, const D3DXVECTOR3& InitDir, float MaxAngle) {
	Bullet::Initialize();
	SetTargetPoint(TargetPos);
	SetDir(InitDir);
	SetMaxAngle(MaxAngle);
	//// 初期位置から目標地点のベクトルを算出。
	//m_ShotToTarget = TargetPos - ShotPos;
	SetStartPos(ShotPos);
	m_RotationAngle = Quaternion::Identity;
}

void CCurveBullet::Update() {
	//// 弾が目標地点よりも奥にあるか判定。
	//// 初期位置から現在の位置へのベクトルを算出。
	//D3DXVECTOR3 ShotToNow = m_transform.position - m_StartPos;
	//// 初期位置から目標地点までの向きベクトル算出。
	//D3DXVECTOR3 ShotToTargetDirection;
	//D3DXVec3Normalize(&ShotToTargetDirection,&m_ShotToTarget);

	//// 初期位置からターゲットへの線上での長さを算出。
	//float Length = D3DXVec3Dot(&ShotToTargetDirection, &ShotToNow);
	//if (Length <= 0.001f) {
	//
	//}


	D3DXVECTOR3 TargetDir;
	// 今の位置から目標位置までの向きベクトルを算出。
	D3DXVec3Normalize(&TargetDir,&(*m_TargetPoint - m_transform.position));

	// 目標向きベクトルと現在の向きベクトルの角度を算出(ラジアン)。
	float rad = acosf(D3DXVec3Dot(&m_NowDir, &TargetDir));	// 向きを持った回転量。
	float unsignedRad = fabsf(rad);	// 向きを持たない回転量。
	float dir = rad / unsignedRad;	// 回転方向。
	float maxRad = D3DXToRadian(m_MaxAngle);	// 外部から受け取った値をラジアンに変換。
	if (maxRad >= unsignedRad) {
		// 1フレームの回転角度のほうが大きい。
		maxRad = rad;
	}
	else {
		// 回転方向をかける。
		maxRad *= dir;
	}

	D3DXVECTOR3 Axis;
	D3DXVec3Cross(&Axis, &m_NowDir, &TargetDir);
	D3DXVec3Normalize(&Axis, &Axis);

	// 角度を用いて弾の向きベクトルを回転。
	D3DXQUATERNION work;
	work = Quaternion::Identity;
	D3DXQuaternionRotationAxis(&work, &Axis, maxRad);

	// クォータニオンから回転行列作成。
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixRotationQuaternion(&mat,&work);

	// 回転行列で向きベクトルを回転。
	D3DXVec3TransformNormal(&m_NowDir, &m_NowDir, &mat);

	// 基底クラスで弾の位置情報を更新。
	Bullet::SetDir(m_NowDir);
	Bullet::Update();
}