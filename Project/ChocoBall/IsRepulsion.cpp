#include "stdafx.h"
#include "IsRepulsion.h"
#include "ObjectManager.h"
#include "BulletPhysics.h"

//// �R�[���o�b�NY(�������[���h)
//// �����[���h�̉����ɓ����������̔���ɗp����B
//struct ContactResult_Y : public btCollisionWorld::ContactResultCallback
//{
//	ContactResult_Y()
//	{
//		isHit = false;
//	}
//
//	// ���炩�̃R���W�����ƏՓ˂������B
//	bool isHit;
//	// �Փ˂����_�B
//	D3DXVECTOR3 hitPos = D3DXVECTOR3(0.0f, -FLT_MAX, 0.0f);
//	// �Փ˂����_�̖@��(Y����)�B
//	D3DXVECTOR3 hitNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	// �����߂������B
//	D3DXVECTOR3 Direction;
//	// �H������ł���傫���B
//	float Length = FLT_MAX;
//
//	D3DXVECTOR3 startPos;
//	// ���������R���W�����̃^�C�v���i�[�B
//	CollisionType hitCollisionType;
//
//	vector<bool>  m_MaskCollisionTypes;	// ���̃N���X�̃C���X�^���X�����I�u�W�F�N�g���A����CollisionType�̂�����𖳎����邩�̃t���O���i�[(true�Ȃ疳��)�B
//	CGameObject* UserPointer = nullptr;		// isIntersect�N���X�̃C���X�^���X��ێ����Ă���I�u�W�F�N�g���i�[����B
//	bool isFirstCallback = false;		// ���̓����蔻��ōŏ��ɌĂ΂ꂽ�R�[���o�b�N���B
//
//	virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
//	{
//		// ������������̑��������������Ă���B
//		int idx = colObj1Wrap->getCollisionObject()->getUserIndex();
//		if (m_MaskCollisionTypes[idx]) {
//			return 0.0f;
//		}
//
//		// �Փ˓_�̖@�������������Ă���B
//		D3DXVECTOR3 hitPointNormal(cp.m_normalWorldOnB.x(), cp.m_normalWorldOnB.y(), cp.m_normalWorldOnB.z());
//		D3DXVec3Normalize(&hitPointNormal, &hitPointNormal);
//
//		// ������Ɩ@���̂Ȃ��p�x���Z�o�B
//		float angle = D3DXVec3Dot(&hitPointNormal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
//		angle = fabsf(acosf(angle));
//		// �n�ʂ̌X�΂�54�x��菬�����ꍇ�n�ʂƂ݂Ȃ��B
//		if (angle >= fPI * 0.3f) {
//			// �Փ˂��Ă������̂��n�ʂł͂Ȃ��B
//			return 0.0f;
//		}
//		isHit = true;
//		//XZ���ʂł̖@���B
//		hitNormal.x = hitPointNormal.x;
//		hitNormal.y = 0.0f;
//		hitNormal.z = hitPointNormal.z;
//		D3DXVec3Normalize(&hitNormal, &hitNormal);
//
//		// �Փ˓_A�B
//		D3DXVECTOR3 posA = static_cast<D3DXVECTOR3>(cp.getPositionWorldOnA());
//		// �Փ˓_B�B
//		D3DXVECTOR3 hitPosTmp = static_cast<D3DXVECTOR3>(cp.getPositionWorldOnB());
//		// �Փ˓_�̋��������߂�B
//		Direction = hitPosTmp - posA;
//		// �����߂��ʁB
//		float fDistTmp = D3DXVec3Length(&Direction);
//		if (Length > fDistTmp) {
//			// ���̏Փ˓_�̂ق����߂��̂ŁA�ł��߂��Փ˓_���X�V����B
//			hitPos = hitPosTmp;
//			hitNormal = static_cast<D3DXVECTOR3>(cp.m_normalWorldOnB);
//			Length = fDistTmp;
//		}
//		// �����Ԃ������B
//		D3DXVec3Normalize(&Direction, &Direction);
//
//		hitCollisionType = static_cast<CollisionType>(colObj1Wrap->getCollisionObject()->getUserIndex());
//
//		if (isFirstCallback) {
//			// ��x�̓����蔻��ň�x�������̏������s��Ȃ��B
//			if (UserPointer) {
//				UserPointer->OnCollisionStay(const_cast<btCollisionObject*>(colObj1Wrap->getCollisionObject()));
//			}
//			if (colObj1Wrap->getCollisionObject()->getUserPointer()) {
//				static_cast<CGameObject*>(colObj1Wrap->getCollisionObject()->getUserPointer())->OnCollisionStay(UserPointer->GetCollisionObject());
//			}
//		}
//		return 0;
//	}
//};

// �R�[���o�b�N(�������[���h)
// �����[���h�̉����ɓ����������̔���ɗp����B
struct ContactResult : public btCollisionWorld::ContactResultCallback
{
	// �Փ˂����_�̖@��()�B
	D3DXVECTOR3 hitNormal;
	// �����̃R���W�����ɓ����������B
	bool isHit = false;
	// ���������R���W�����̃^�C�v���i�[�B
	CollisionType hitCollisionType;
	// �Փ˓_�B
	D3DXVECTOR3 hitPos;
	// �����߂������B
	D3DXVECTOR3 Direction;
	// �H������ł���傫���B
	float Length = 0.0f;
	ContactResult()
	{
		isHit = false;
	}

	vector<bool>  m_MaskCollisionTypes;	// ���̃N���X�̃C���X�^���X�����I�u�W�F�N�g���A����CollisionType�̂�����𖳎����邩�̃t���O���i�[(true�Ȃ疳��)�B
	CGameObject* UserPointer = nullptr;		// isIntersect�N���X�̃C���X�^���X��ێ����Ă���I�u�W�F�N�g���i�[����B
	bool isFirstCallback = false;		// ���̓����蔻��ōŏ��ɌĂ΂ꂽ�R�[���o�b�N���B

	virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
	{
		// ������������̑��������������Ă���B
		int idx = colObj1Wrap->getCollisionObject()->getUserIndex();
		if (m_MaskCollisionTypes[idx]) {
			return 0.0f;
		}

		// �Փ˓_�̖@�������������Ă���B
		D3DXVECTOR3 hitPointNormal(cp.m_normalWorldOnB.x(), cp.m_normalWorldOnB.y(), cp.m_normalWorldOnB.z());
		D3DXVec3Normalize(&hitPointNormal, &hitPointNormal);

		//float d = D3DXVec3Dot(&hitPointNormal, &CVec3Up);
		//if (acosf(d) < fPI * 0.2) {
		//	//�z���g�͒n�ʂ��ǂ����Ƃ��̑���������̂��x�X�g�Ȃ񂾂��ǁA����͊p�x�ŁB
		//	return 0.0f;
		//}
		isHit = true;
		//XZ���ʂł̖@���B
		hitNormal.x = hitPointNormal.x;
		hitNormal.y = hitPointNormal.y;
		hitNormal.z = hitPointNormal.z;
		D3DXVec3Normalize(&hitNormal, &hitNormal);

		// �Փ˓_A�B
		D3DXVECTOR3 posA = static_cast<D3DXVECTOR3>(cp.getPositionWorldOnA());
		// �Փ˓_B�B
		hitPos = static_cast<D3DXVECTOR3>(cp.getPositionWorldOnB());
		// �����Ԃ������B
		Direction = hitPos - posA;
		Direction.y = 0.0f;
		// �����߂��ʁB
		Length = D3DXVec3Length(&Direction);
		D3DXVec3Normalize(&Direction, &Direction);

		hitCollisionType = static_cast<CollisionType>(colObj1Wrap->getCollisionObject()->getUserIndex());

		if (isFirstCallback) {
			// ��x�̓����蔻��ň�x�������̏������s��Ȃ��B
			if (UserPointer) {
				UserPointer->OnCollisionStay(const_cast<btCollisionObject*>(colObj1Wrap->getCollisionObject()));
			}
			if (colObj1Wrap->getCollisionObject()->getUserPointer()) {
				static_cast<CGameObject*>(colObj1Wrap->getCollisionObject()->getUserPointer())->OnCollisionStay(UserPointer->GetCollisionObject());
			}
		}
		return 0.0f;
	}
};



CIsRepulsion::CIsRepulsion()
{
}


CIsRepulsion::~CIsRepulsion()
{
}

void CIsRepulsion::Initialize(btCollisionObject* collision) {
	m_pCollision = collision;
}

void CIsRepulsion::IsRepulsion(const D3DXVECTOR3& pos, D3DXVECTOR3& moveSpeed){
	//ContactResult callback;
	//callback.m_MaskCollisionTypes =
	//	callback.isFirstCallback = false;
	//callback.UserPointer = static_cast<CGameObject*>(m_pCollision->getUserPointer());
	//SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ContactTest_Dynamic(m_pCollision,callback);
	//if (callback.isHit) {

	//}
}
