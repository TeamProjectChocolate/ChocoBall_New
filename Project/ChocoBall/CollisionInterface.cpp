#include "stdafx.h"
#include "CollisionInterface.h"
#include "ObjectManager.h"
#include "BulletPhysics.h"
#include "Infomation.h"

CCollisionInterface::CCollisionInterface()
{
	// �I�u�W�F�N�g�}�l�[�W���[����BulletPhysics�N���X�̃C���X�^���X��ۑ��B
	m_pBulletPhysics = SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"));
	m_transform.Identity();
}

CCollisionInterface::~CCollisionInterface()
{
	// ���ȉ��̃R�����g�A�E�g���Ă���֐��͏������z�֐��ƂȂ��Ă���A�p����ŏ㏑������Ă���B
	//   �������p���N���X�̃f�X�g���N�^�����ɌĂ΂�邽�߁A���N���X�̃f�X�g���N�^���Ă΂��Ƃ��ɂ͂��łɊ֐��{�̂̃������̈�͉������Ă���B
	//   ���̂��ߊ��N���X�ɂ��̏������������ꍇ�͖������V���{���ƂȂ�R���p�C���G���[�ƂȂ�B
	//   �|�����[�t�B�Y���͊��p�ł��Ȃ��B
	// ���܂��ȉ��̊֐�������ŌĂяo�����̊֐������N���X�ł͌Ă΂Ȃ��B
	//   �Ă�ł��܂����ꍇ�̓R���p�C���G���[�ƂȂ炸�v���O���������s����A���N���X�̃f�X�g���N�^���Ă΂ꂽ�u�ԂɃ������O�A�N�Z�X����������B

	//RemoveWorld();
}

void CCollisionInterface::InitCollision(
	CGameObject* pObject,
	const SH_ENGINE::TRANSFORM& Transform,
	const D3DXVECTOR3& offset,
	btCollisionShape* pShape,
	Collision::Type Type,
	Collision::FilterGroup group,
	float mass,
	bool flg,
	bool isAddWorld)
{
	// �R���W�����ƃ��f���̒��S���W�̍������i�[�B
	m_OriginOffset = btVector3(offset.x, offset.y, offset.z);
	//�����ō��̂̌`�������B
	m_collisionShape.reset(pShape);

	// �R���W�����̑����ۑ��B
	SetCollisionType(Type);
	// �t�B���^�[�O���[�v�ۑ��B
	SetFilterGroup(group);
	// �f�t�H���g�ł͂��ׂĂ̑����̃R���W�����Ɠ����蔻������B
	// �����������ׂĂ̂��̂Ɠ����蔻����s���Əd�����߁A�󋵂ɉ����ĊO������}�X�N��ݒ肵�Ă��������B
	this->BitMask_AllOn();

	btTransform rbTransform;
	rbTransform.setIdentity();
	rbTransform.setOrigin(btVector3(Transform.position.x, Transform.position.y, Transform.position.z) + m_OriginOffset);
	rbTransform.setRotation(btQuaternion(Transform.angle.x, Transform.angle.y, Transform.angle.z, Transform.angle.w));

	// Transform����ۑ��B
	m_transform = Transform;
	m_transform.scale = D3DXVECTOR3(pShape->getLocalScaling());
	{
		// �������z�֐��B
		Build(rbTransform, mass, flg);
		if (isAddWorld) {
			// �������z�֐��B
			// ���������R���W���������[���h�ɓo�^�B
			AddWorld();
		}
	}

	// �R���W���������I�u�W�F�N�g�̃|�C���^��ݒ�B
	m_collisionObject->setUserPointer(pObject);
	m_collisionObject->setUserIndex(static_cast<int>(m_MyType));
	m_collisionObject->setActivationState(DISABLE_DEACTIVATION);
	m_collisionObject->activate();
}

