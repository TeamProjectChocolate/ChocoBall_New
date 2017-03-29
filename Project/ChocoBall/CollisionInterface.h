#pragma once
#include "stdafx.h"
#include "btBulletDynamicsCommon.h"
#include "CollisionType.h"
#include "Infomation.h"

class CGameObject;

class CBulletPhysics;

// �����蔻��╨�������ɗp����N���X�̃C���^�[�t�F�[�X�B
// �e���v���[�g���\�b�h�p�^�[���̋K��N���X�Ƃ��Ďg�p�B
class CCollisionInterface
{
public:
	CCollisionInterface();
	virtual ~CCollisionInterface();
	// �p�����[�^���Q�Ƃ��ăR���W�����𐶐��B
	// �����F	 ���̍��̂����I�u�W�F�N�g�̃|�C���^�B
	//			 �R���W�����ɐݒ肷��g�����X�t�H�[�����B
	//			 ���f���̌��_�ƃR���W�����̒��S�̍����B
	//			 �O���Œ�`�����R���W�����̌`��B
	//			 �R���W�����̑���(������BulletPhysics��UserIndex�ɓo�^�����)�B
	//			 ���̃R���W�����̃t�B���^�[�O���[�v(Collision::FilterGroup�̒l��ݒ�)�B
	//			 ����(isKinematic��true�Ȃ�Ύ����I��0.0f�ɂȂ�)�B
	//			 �t���O(�p����ŏڍ׌���)�B
	//			 ���������ケ�̊֐����Ń��[���h�ɓo�^���邩(true�œo�^)�B
	// �����̃N���X�ł͑�܂��ȏ����̕������Ƌ��ʏ����̂ݎ����B
	void InitCollision(
		CGameObject* pObject,
		const SH_ENGINE::TRANSFORM& Transform,
		const D3DXVECTOR3& offset,
		btCollisionShape* pShape,
		Collision::Type Type,
		Collision::FilterGroup group,
		float mass,
		bool,
		bool isAddWorld);

	virtual void Update(D3DXVECTOR3* pos, D3DXQUATERNION* rot);

	// �������[���h�ŏՓ˂�����Ă΂��R�[���o�b�N(XZ����)�B
	void OnCollisionStay_XZ(CCollisionInterface* pCollision);
	// �������[���h�ŏՓ˂�����Ă΂��R�[���o�b�N(Y����)�B
	void OnCollisionStay_Y(CCollisionInterface* pCollision);
	// �R���W�������[���h�ŏՓ˂�����Ă΂��R�[���o�b�N�B
	void OnTriggerStay(CCollisionInterface* pCollision);

	// �S���C���[�}�X�N�I�t�B
	// ���ׂĂ̏Փ˂𖳎��B
	__inline void BitMask_AllOff() {
		SetLayerMask(0x00000000);
	}

	// �S���C���[�}�X�N�I���B
	// ���ׂẴR���W�����Ɠ����蔻����s���B
	__inline void BitMask_AllOn() {
		SetLayerMask(btBroadphaseProxy::CollisionFilterGroups::AllFilter);
	}

	// �w�肵���R���W�����^�C�v�̃��C���[�}�X�N�I�t�B
	// �w�肵���R���W�����Ƃ̏Փ˖����B
	__inline void BitMask_Off(Collision::FilterGroup group) {
		// type���ڂ̃r�b�g��0�ɂ��ă}�X�N���I�t�ɂ���B
		int mask = 0x00000001;	// 0x00000001�B
		// type�������V�t�g���Ă�������ړI��bit��1�ɂ���B
		mask = mask << static_cast<int>(group);	// 0x00000001�B
		// ���ׂĂ�bit�𔽓]���A�ړI�̃r�b�g�̂�0�A����1�ɂ���B
		mask = ~mask;	// 0xfffffffe�B

		// AND���Z�ōŏI�I�ȃ}�X�Nbit���Z�o�B
		SetLayerMask(GetLayerMask() & mask);
	}

	// �w�肵���R���W�����^�C�v�̃��C���[�}�X�N�I���B
	__inline void BitMask_On(Collision::FilterGroup group) {
		// type���ڂ̃r�b�g��1�ɂ��ă}�X�N���I�t�ɂ���B
		int mask = 0x00000001;	// 0x00000001�B
		// type�������V�t�g�B
		mask = mask << static_cast<int>(group);

		// OR���Z�Ō��̃}�X�N�ɉ��Z�B
		SetLayerMask(GetLayerMask() | mask);
	}

	// �R���W�������A�N�e�B�x�[�g�B
	inline void Activate() {
		m_collisionObject->activate();
	}
	// ���̃R���W���������[���h�ɒǉ�����B
	virtual void AddWorld() = 0;

	// ���̃R���W���������[���h����폜����B
	virtual void RemoveWorld() = 0;


	// ���̃R���W���������Q�[���I�u�W�F�N�g��ԋp�B
	inline CGameObject* GetGameObject() const
	{
		return m_GameObject;
	}
	// �������肩���擾�B
	inline bool GetIsCollisionXZ() const {
		return m_OnCollisionXZ;
	}
	// �c�����肩���擾�B
	inline bool GetIsCollisionY() const {
		return m_OnCollisionY;
	}

	// �R���W�����̑����ݒ�B
	// �����̈�����btCollisionObject��userIndex�ɐݒ肳���B
	// ���R���W�����ւ̑����ݒ�͕K�����̊֐��ōs���悤�ɁB
	inline void SetCollisionType(Collision::Type type) {
		m_MyType = type;
		if (m_collisionObject) {
			m_collisionObject->setUserIndex(static_cast<int>(type));
		}
	}
	// �R���W�����̑����擾�B
	inline Collision::Type GetCollisionType() const
	{
		return m_MyType;
	}

	// �����̂�R���W�����I�u�W�F�N�g�ւ̃t�B���^�[�O���[�v�ݒ�͕K�����̊֐��ōs���Ă��������B
	// ������btCollisionObject�̒l��ύX���Ȃ��悤�ɁI�B
	inline void SetFilterGroup(Collision::FilterGroup group) {
		m_UserFilterGroup = group;
		// bit�`���ɕϊ����ĕۑ��B
		m_FilterGroup = TypeToBit(group);
		// BulletPhysics�Ɏ����̑�����ݒ�B
		ConfigCollisionFilterGroup();
	}
	// �t�B���^�[�O���[�v�擾�B
	inline Collision::FilterGroup SetFilterGroup() const
	{
		return m_UserFilterGroup;
	}

	// �R���W�����I�u�W�F�N�g�{�̂�ԋp�B
	inline const btCollisionObject* GetCollisionObject() const {
		return m_collisionObject.get();
	}

	// �R���W�����`���ԋp�B
	inline btCollisionShape* GetCollisionShape() const
	{
		return m_collisionShape.get();
	}

	// ���C���[�}�X�N�ԋp�B
	// �߂�l��bit�`���̃}�X�N�B
	inline int GetLayerMask() const {
		return m_LayerMask;
	}

	// Transform���擾�B
	inline const SH_ENGINE::TRANSFORM& GetTransform() const{
		return m_transform;
	}

	// �R���W�����{�̂Ɉʒu���ݒ�B
	inline void SetPos(const D3DXVECTOR3& pos) {
		m_transform.position = pos;
		m_collisionObject->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
	}
	// �R���W�����{�̂̈ʒu���(���S�_)�ԋp�B
	inline const D3DXVECTOR3& GetPos() const {
		return 	m_transform.position;
	}

	// �R���W�����{�̂̉�]���擾�B
	inline const D3DXQUATERNION& GetRotation()const {
		return m_transform.angle;
	}

	// �R���W�����`��{�̂ɃR���W�����T�C�Y�ݒ�B
	inline void SetScale(const D3DXVECTOR3& scale) {
		m_transform.scale = scale;
		m_collisionObject->getCollisionShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
	}
	// �R���W�����`��{�̂ɃR���W�����T�C�Y�ԋp(����)�B
	inline const D3DXVECTOR3& GetScale()const {
		return 	m_transform.scale;
	}

protected:

	// ���C���[�}�X�N��n���B
	__inline void ConfigCollisionFilterMask() {
		if (m_collisionObject) {
			// �R���W�����I�u�W�F�N�g����������Ă���B
			btBroadphaseProxy* Proxy = m_collisionObject->getBroadphaseHandle();
			if (Proxy) {
				// �u���[�h�t�F�[�Y�n���h������������Ă���B
				Proxy->m_collisionFilterMask = m_LayerMask;
			}
		}
	}

	// �R���W�����̑�����n���B
	__inline void ConfigCollisionFilterGroup() {
		if (m_collisionObject) {
			// �R���W�����I�u�W�F�N�g����������Ă���B
			// �R���W�����I�u�W�F�N�g�ɂ��̂܂܂̑�����ݒ�B
			m_collisionObject->setUserIndex(static_cast<int>(m_MyType));
			btBroadphaseProxy* Proxy = m_collisionObject->getBroadphaseHandle();
			if (Proxy) {
				// �u���[�h�t�F�[�Y�n���h������������Ă���B
				Proxy->m_collisionFilterGroup = m_FilterGroup;
			}
		}
	}


private:
	// ���ۂɃR���W�����I�u�W�F�N�g�𐶐�����֐��B
	// �p����̃N���X�ɏڍ׏������Ϗ��B
	virtual void Build(
		const btTransform& transform,
		float mass,
		bool) = 0;


	// �󂯎�����R���W�����̑������r�b�g��ł̑����ɕϊ����ĕԋp�B
	__inline int TypeToBit(Collision::FilterGroup group) {
		//if (static_cast<int>(type) >= 10) {
		//	// �}�X�N�ɐݒ�ł���̂̓f�t�H���g�̃}�X�N��������26bit�܂ŁB
		//	//abort();
		//	type = CollisionType::Boss_Cource;
		//}
		int ret = 0x00000001;
		// �^�C�v�̐��������V�t�g�B
		ret = ret << static_cast<int>(group);
		return ret;
	}

	// ���C���[�}�X�N�Z�b�g�B
	// ������bit�`���̃}�X�N�B
	// �����̊֐����Ă񂾂�����̒l�����C���[�}�X�N�Ƃ���BulletPhysics�ɒʒm����܂��B
	__inline void SetLayerMask(int mask) {
		m_LayerMask = mask;
		ConfigCollisionFilterMask();
	}
protected:
	//��������bulletPhysics�̍��̂��g�p���邽�߂ɕK�v�ȕϐ��B
	btVector3 m_OriginOffset;		// �I�u�W�F�N�g�̒��S�ƍ��̂̒��S�̍����B
	unique_ptr<btCollisionShape>	m_collisionShape;	//�R���W�����̌`��B
	unique_ptr<btCollisionObject>		m_collisionObject;		//���́B
	Collision::Type m_MyType = Collision::Type::None;	// �R���W�����̑����B
	Collision::FilterGroup m_UserFilterGroup = Collision::FilterGroup::None;	// �R���W�����̃t�B���^�[�O���[�v(���[�U�[���ݒ�)�B
	int m_FilterGroup = 0x00000000;	// �R���W�����̃t�B���^�[�O���[�v(���̃N���X���ł̂ݎg�p)�B
	// ������𖳎����鑮����bit���Ŋi�[(�Փ˂��鑮����bit��1�𗧂Ă�)�B
	// ���R���W���������[���h�ɓo�^�����Ƃ��ɂ͂��߂Ēʒm����B
	int m_LayerMask = btBroadphaseProxy::CollisionFilterGroups::AllFilter;
protected:
	CBulletPhysics* m_pBulletPhysics;	// �������[���h�ƃR���W�������[���h�����N���X�B
	bool m_OnCollisionXZ = false;	// XZ�����ŏՓ˂������B
	bool m_OnCollisionY = false;	// Y�����ŏՓ˂������B

	CGameObject* m_GameObject = nullptr;
	SH_ENGINE::TRANSFORM m_transform;
};
