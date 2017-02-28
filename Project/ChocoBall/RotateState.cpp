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

		// �ŏ��̌����x�N�g���ƖڕW�̌����x�N�g���̊p�x���Z�o�B
		m_Angle = acosf(D3DXVec3Dot(&InitDirection, &m_TargetDirection));

		if (fabsf(m_Angle) <= 0.001f) {
			// ��̃x�N�g���ɒ�������x�N�g�����Z�o(�N�H�[�^�j�I���̉�]���Ƃ��Ďg�p)�B
			D3DXVec3Cross(&m_Axis, &InitDirection, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
		}
		else if (fabsf((fabsf(m_Angle) - D3DXToRadian(180.0f))) <= 0.001f) {
			// ��̃x�N�g���ɒ�������x�N�g�����Z�o(�N�H�[�^�j�I���̉�]���Ƃ��Ďg�p)�B
			D3DXVec3Cross(&m_Axis, &InitDirection, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
		}
		else {
			// ��̃x�N�g���ɒ�������x�N�g�����Z�o(�N�H�[�^�j�I���̉�]���Ƃ��Ďg�p)�B
			D3DXVec3Cross(&m_Axis, &InitDirection, &m_TargetDirection);
		}
		D3DXVec3Normalize(&m_Axis, &m_Axis);

		// �ڕW�x�N�g���܂ŉ�]����N�H�[�^�j�I���쐬�B
		m_InitQuat = m_pObject->GetQuaternion();
		m_Offset = 0.0f;
		m_IsFirst = false;
	}
	// 1�t���[���ɉ�]���銄�����Z�o���ĉ��Z�B
	m_Offset += (1.0f / 60.0f) / (m_RotationTime);

	// �Z�o���������ŏ��X�ɉ�]�B
	D3DXQUATERNION work;
	D3DXQuaternionRotationAxis(&work, &m_Axis, m_Angle * m_Offset);
	

	D3DXMATRIX mat, mat2;
	D3DXQUATERNION work2 = work;
	work = m_InitQuat * work;

	D3DXVECTOR3 dirX, dirY, dirZ;

	// ��]�ɂ���ă{�X���X���Ȃ��悤�ɒ����B
	{
		D3DXMatrixIdentity(&mat);
		D3DXMatrixRotationQuaternion(&mat, &work);
		dirZ.x = mat.m[2][0];
		dirZ.y = mat.m[2][1];
		dirZ.z = mat.m[2][2];

		D3DXVec3Normalize(&dirZ, &dirZ);
		// ���̌����ƃ��[���h�ł̏�����Ƃ̊O�ς����߁A���[�J���ł�X���ɑ������B
		D3DXVec3Cross(&dirX, &dirZ, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		D3DXVec3Normalize(&dirX, &dirX);
		dirX *= -1.0f;
		//dirZ *= -1.0f;
		// ���̌����ƃ��[�J���ł�X�����烍�[�J�����W�ł̏�������Z�o�B
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

