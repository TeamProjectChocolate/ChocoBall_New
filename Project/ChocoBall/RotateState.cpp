#include "stdafx.h"
#include "RotateState.h"


CRotateState::~CRotateState()
{
}

void CRotateState::Entry() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::Wait);
	m_pObject->SetPlayingState(PLAYING_STATE::REPEAT);
	m_RotationTime = 0.0f;
	m_TimeCounter = 0.0f;
	m_IsFirst = true;
}

bool CRotateState::Update() {
	m_TimeCounter += 1.0f / 60.0f;
	if (m_IsFirst) {
		D3DXVECTOR3 InitDirection = m_pObject->GetDirection();

		// 最初の向きベクトルと目標の向きベクトルの角度を算出。
		m_Angle = acosf(D3DXVec3Dot(&InitDirection, &m_TargetDirection));

		if (fabsf(m_Angle) <= 0.001f) {
			// 二つのベクトルに直交するベクトルを算出(クォータニオンの回転軸として使用)。
			D3DXVec3Cross(&m_Axis, &InitDirection, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
		}
		else if (fabsf((fabsf(m_Angle) - D3DXToRadian(180.0f))) <= 0.001f) {
			// 二つのベクトルに直交するベクトルを算出(クォータニオンの回転軸として使用)。
			D3DXVec3Cross(&m_Axis, &InitDirection, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
		}
		else {
			// 二つのベクトルに直交するベクトルを算出(クォータニオンの回転軸として使用)。
			D3DXVec3Cross(&m_Axis, &InitDirection, &m_TargetDirection);
		}
		D3DXVec3Normalize(&m_Axis, &m_Axis);

		// 目標ベクトルまで回転するクォータニオン作成。
		m_InitQuat = m_pObject->GetQuaternion();
		m_Offset = 0.0f;
		m_IsFirst = false;
	}
	// 1フレームに回転する割合を算出して加算。
	m_Offset += (1.0f / 60.0f) / (m_RotationTime);

	// 算出した割合で徐々に回転。
	D3DXQUATERNION work;
	D3DXQuaternionRotationAxis(&work, &m_Axis, m_Angle * m_Offset);
	

	D3DXMATRIX mat, mat2;
	D3DXQUATERNION work2 = work;
	work = m_InitQuat * work;

	D3DXVECTOR3 dirX, dirY, dirZ;

	// 回転によってボスが傾かないように調整。
	{
		D3DXMatrixIdentity(&mat);
		D3DXMatrixRotationQuaternion(&mat, &work);
		dirZ.x = mat.m[2][0];
		dirZ.y = mat.m[2][1];
		dirZ.z = mat.m[2][2];

		D3DXVec3Normalize(&dirZ, &dirZ);
		// 今の向きとワールドでの上方向との外積を求め、ローカルでのX軸に代入する。
		D3DXVec3Cross(&dirX, &dirZ, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		D3DXVec3Normalize(&dirX, &dirX);
		dirX *= -1.0f;
		//dirZ *= -1.0f;
		// 今の向きとローカルでのX軸からローカル座標での上方向を算出。
		D3DXVec3Cross(&dirY, &dirX, &dirZ);
		D3DXVec3Normalize(&dirY, &dirY);
		dirY *= -1.0f;

		mat2.m[0][0] = dirX.x;
		mat2.m[0][1] = dirX.y;
		mat2.m[0][2] = dirX.z;
		mat2.m[1][0] = dirY.x;
		mat2.m[1][1] = dirY.y;
		mat2.m[1][2] = dirY.z;
		mat2.m[2][0] = dirZ.x;
		mat2.m[2][1] = dirZ.y;
		mat2.m[2][2] = dirZ.z;

		D3DXQuaternionRotationMatrix(&work2, &mat2);
	}

	m_pObject->SetQuaternion(work2);

	if (m_TimeCounter >= m_RotationTime) {
		return true;
	}
	return false;
}

void CRotateState::Exit(CEnemy_Boss::BOSS_STATE next) {

}

