#include "stdafx.h"
#include "IsRepulsion.h"
#include "ObjectManager.h"
#include "BulletPhysics.h"


CIsRepulsion::CIsRepulsion()
{
}


CIsRepulsion::~CIsRepulsion()
{
}

void CIsRepulsion::Initialize(CCollisionInterface* collision) {
	m_pCollision = collision;
}

void CIsRepulsion::Repulsion(D3DXVECTOR3* moveSpeed){
	ContactResult callback;
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ContactTest_Dynamic(m_pCollision->GetCollision(),callback);
	if (callback.isHit) {
		// �������Ă���B
		if (callback.hitCollisionType != Collision::Type::AttackWall) {
			// �U�����Ă���`���R�ǂ���Ȃ���ΏI���B
			return;
		}
		// �Փ˓_�B
		D3DXVECTOR3 hitPoint = callback.hitPoint;
		// �Փ˓_���炱�̃N���X�����R���W�����̒��S�ւ̃x�N�g���B
		D3DXVECTOR3 Vec = m_pCollision->GetPos() - hitPoint;
		// �Փ˓_���璆�S�_�ւ̋����B
		float Dist;
		Dist = D3DXVec3Length(&Vec);
		// �R���W�����̑傫�����狗���������čŏI�I�ɉ����߂��ʂ����߂�B
		Dist = m_pCollision->GetScale().x - Dist;

		// �H�����񂾋t�����ɎZ�o�����ʉ����߂��B
		*moveSpeed += (Vec * Dist);
	}
}
