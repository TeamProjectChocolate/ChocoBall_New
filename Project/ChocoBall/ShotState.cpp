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

			// �e�̔����ʒu�����������ɂ΂炷�B
			D3DXVECTOR3 pos = m_pObject->GetPos() + (BuletShotOffsetDir[idx]) * 3.0f;

			// ��]�ʐݒ�B
			float MaxAngle = 0.7f;

			// �e�̌����ݒ�B
			D3DXVECTOR3 Dir;
			// �p�x��p���Ēe�̌����x�N�g������]�B
			D3DXQUATERNION work;
			work = Quaternion::Identity;
			D3DXQuaternionRotationAxis(&work, &Vector3::AxisY, D3DXToRadian(15.0f) * m_RotationDir);
			// �N�H�[�^�j�I�������]�s��쐬�B
			D3DXMATRIX mat;
			D3DXMatrixIdentity(&mat);
			D3DXMatrixRotationQuaternion(&mat, &work);

			// ��]�s��Ō����x�N�g������]�B
			D3DXVec3TransformNormal(&Dir, &m_pObject->GetDirection(), &mat);

			b->Initialize(pos,m_pPlayer->GetPosRef(), Dir,MaxAngle);

			// �e�̑��x�ݒ�B
			b->SetBulletSpeed(0.7f);
			// �˒��ݒ�B
			b->SetRange(50.0f);

			// �e�̋�����ύX�B
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

