#include "stdafx.h"
#include "ShotState.h"
#include "Player.h"
#include "EnemyBullet.h"
#include "Enemy_Boss.h"
#include "CurveBullet.h"

CShotState::CShotState(CEnemy_Boss* obj) : CState(obj) {
	m_pPlayer = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));
}

CShotState::~CShotState()
{
}

void CShotState::Entry() {
	m_TimeCounter = 0.0f;
	m_RotationDir = 1;
}

bool CShotState::Update() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::FastsSwim);
	m_pObject->SetPlayingState(ANIMATION::PLAYING_STATE::ONCE);
	m_TimeCounter += 1.0f / 60.0f;
	if (m_TimeCounter >= m_IntervalTime) {
		for (int idx = 0; idx < 4; idx++) {
			CEnemyBullet* bullet = SINSTANCE(CObjectManager)->GenerationObject<CEnemyBullet>(_T("EnemyBaullet"), OBJECT::PRIORTY::OBJECT3D, false);
			bullet->Initialize();

			CCurveBullet* b = new CCurveBullet;

			// 弾の発生位置をいい感じにばらす。
			D3DXVECTOR3 pos = m_pObject->GetPos() + (BuletShotOffsetDir[idx]) * 3.0f;

			// 回転量設定。
			float MaxAngle = 0.7f;

			// 弾の向き設定。
			D3DXVECTOR3 Dir;
			// 角度を用いて弾の向きベクトルを回転。
			D3DXQUATERNION work;
			work = Quaternion::Identity;
			D3DXQuaternionRotationAxis(&work, &Vector3::AxisY, D3DXToRadian(15.0f) * m_RotationDir);
			// クォータニオンから回転行列作成。
			D3DXMATRIX mat;
			D3DXMatrixIdentity(&mat);
			D3DXMatrixRotationQuaternion(&mat, &work);

			// 回転行列で向きベクトルを回転。
			D3DXVec3TransformNormal(&Dir, &m_pObject->GetDirection(), &mat);

			b->Initialize(pos,m_pPlayer->GetPosRef(), Dir,MaxAngle);

			// 弾の速度設定。
			b->SetBulletSpeed(0.7f);
			// 射程設定。
			b->SetRange(50.0f);

			// 弾の挙動を変更。
			bullet->ChangeBulletClass(b);

		}
		m_RotationDir *= -1;
		m_TimeCounter = 0.0f;
	}
	if (m_pObject->GetAnimation()->GetIsOnceEnd()) {
		return true;
	}
	return false;
}

void CShotState::Exit(CEnemy_Boss::BOSS_STATE next) {

}

