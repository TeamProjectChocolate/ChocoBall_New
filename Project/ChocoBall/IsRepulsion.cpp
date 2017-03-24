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
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ContactTest_Dynamic(m_pCollision->GetCollisionObject(),callback);

	// ���̂������I�ɓ����Ă��Ȃ��B
	// �ق�̏��������R���W�����T�C�Y��傫�����ċ^���I�ɑS���ʂ�SweepTest��������B
	D3DXVECTOR3 OriginScale = m_pCollision->GetScale();
	m_pCollision->SetScale(OriginScale * 2.0f);

	if (callback.isHit) {
		// �R���W�����T�C�Y�����ɖ߂��B
		m_pCollision->SetScale(OriginScale);

		if (callback.hitCollisionType_A == Collision::Type::Map || callback.hitCollisionType_B == Collision::Type::Map) {
			return;
		}
		// �������Ă���B
		if ((callback.hitCollisionType_A != Collision::Type::AttackWall) && (callback.hitCollisionType_B != Collision::Type::AttackWall)) {
			// �U�����Ă���`���R�ǂ���Ȃ���ΏI���B
			return;
		}

		OutputDebugString("�����ǂɏՓˁB\n");

		// �Փ˓_�B
		D3DXVECTOR3 hitPoint;

		// �`���R�ǂ̏��������Ă���B
		if (callback.hitCollisionType_A == Collision::Type::AttackWall) {
			hitPoint = callback.hitPoint_A;
		}
		else {
			hitPoint = callback.hitPoint_B;
		}

		// Y������ł������B
		D3DXVECTOR3 pos = m_pCollision->GetPos();
		pos.y = hitPoint.y;

		// �Փ˓_���炱�̃N���X�����R���W�����̒��S�ւ̃x�N�g���B
		D3DXVECTOR3 Vec = pos - hitPoint;
		// �Փ˓_���璆�S�_�ւ̋����B
		float Dist;
		Dist = D3DXVec3Length(&Vec);
		// �R���W�����̑傫�����狗���������čŏI�I�ɉ����߂��ʂ����߂�B
		Dist = m_pCollision->GetScale().x - Dist;

		// �H�����񂾋t�����ɎZ�o�����ʉ����߂��B
		D3DXVec3Normalize(&Vec, &Vec);
		*moveSpeed += (Vec * Dist);
	}
	else {
		// �R���W�����T�C�Y��߂��B
		m_pCollision->SetScale(OriginScale);
	}

}
