#include "stdafx.h"
#include "MoveState.h"
#include "Enemy_Boss.h"
#include "TransState.h"
#include "RotateState.h"

CMoveState::~CMoveState()
{
}

void CMoveState::Entry() {
	m_pObject->SetAnimationState(CEnemy_Boss::BOSS_ANIMATION::Swim);
	m_pObject->SetPlayingState(PLAYING_STATE::REPEAT);

	// �R�[�X��`�����]�X�e�[�g�̃p�����[�^������B
	vector<BOSS_COURCE_BLOCK*> nowCource = m_pObject->GetNowCource();
	D3DXVECTOR3 dir = D3DXVECTOR3(0.0f,0.0f,0.0f);
	if (nowCource.size() == 1 && nowCource[0]->BlockType != Boss_Cource::BOSS_COURCE_TYPE::Move) {
		// �A�N�V�������s���ʒu�ւ̌������擾�B
		D3DXVec3Normalize(&dir, &(nowCource[0]->ActionPos - m_pObject->GetPos()));
	}
	else {
		int idx = 0;
		for (auto now : nowCource) {
			D3DXVECTOR3 work;
			D3DXVec3Normalize(&work, &(now->endPosition - now->startPosition));
			dir += work;
			idx++;
		}
		// ���ݕ����̃R�[�X�ɏd�Ȃ��Ă���ꍇ�̓n�[�t�x�N�g�����Z�o�B
		dir /= idx;
	}

	// ��]�X�e�[�g�Ƀp�����[�^��ݒ�B
	this->ChangeLocalState(CEnemy_Boss::BOSS_STATE::Rotate);
	// �R�[�X�̌������^�[�Q�b�g�ɐݒ�B
	static_cast<CRotateState*>(m_pCurrentLocalState)->SetTargetDirection(dir);
	static_cast<CRotateState*>(m_pCurrentLocalState)->SetRotationTime(1.0f);

	// �o���A�N���B
	m_pObject->GetBarrier()->OnBarrier();
}

bool CMoveState::Update() {
	bool IsEnd = m_pCurrentLocalState->Update();
	if (IsEnd) {
		// ���[�J���X�e�[�g�̏��������������B
		switch (m_NowLocalState) {
		case CEnemy_Boss::BOSS_STATE::Rotate:
			this->ChangeLocalState(CEnemy_Boss::BOSS_STATE::Trans);
			static_cast<CTransState*>(m_pCurrentLocalState)->SetMoveSpeed(0.125f);
			if (m_pObject->GetNowCource().size() == 1) {
				// �ڐG���Ă���R�[�X����̂݁B
				BOSS_COURCE_BLOCK* block = m_pObject->GetNowCource()[0];
				if (block->BlockType != Boss_Cource::BOSS_COURCE_TYPE::Move) {
					// �R�[�X�̑������ړ��ł͂Ȃ��B
					float length = D3DXVec3Length(&(m_pObject->GetPos() - block->ActionPos));
					static_cast<CTransState*>(m_pCurrentLocalState)->SetLengthMax(length);
				}
			}
			break;
		case CEnemy_Boss::BOSS_STATE::Trans:
			m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BWait);
			return true;
			break;
		}
	}
	else {
		if (m_pObject->GetIsNowCourceChange()) {
			// �ړ��������ʌ��݂̃R�[�X�ɕύX���������B
			m_pObject->ChangeState(CEnemy_Boss::BOSS_STATE::BMove);
		}
	}
	return false;
}

void CMoveState::Exit(CEnemy_Boss::BOSS_STATE next) {

}
