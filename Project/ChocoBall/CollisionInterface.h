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
	//			 ����(isKinematic��true�Ȃ�Ύ����I��0.0f�ɂȂ�)�B
	//			 �t���O(�p����ŏڍ׌���)�B
	//			 ���������ケ�̊֐����Ń��[���h�ɓo�^���邩(true�œo�^)�B
	// �����̃N���X�ł͑�܂��ȏ����̕������Ƌ��ʏ����̂ݎ����B
	void InitCollision(
		CGameObject* pObject,
		const SH_ENGINE::TRANSFORM& Transform,
		const D3DXVECTOR3& offset,
		btCollisionShape* pShape,
		CollisionType Type,
		float mass,
		bool,
		bool isAddWorld);

	virtual void Update(D3DXVECTOR3* pos, D3DXQUATERNION* rot) = 0;

	void Draw();

	// �S���C���[�}�X�N�I���B
	__inline void BitMask_AllOn() {
		SetLayerMask(0x80000000);
		// �}�X�N�ݒ�B
		ConfigCollisionFilterMask();
	}

	// �S���C���[�}�X�N�I�t�B
	// ���ׂẴR���W�����Ɠ����蔻����s���B
	__inline void BitMask_AllOff() {
		SetLayerMask(btBroadphaseProxy::CollisionFilterGroups::AllFilter);
		// �}�X�N�ݒ�B
		ConfigCollisionFilterMask();
	}

	// �w�肵���R���W�����^�C�v�̃��C���[�}�X�N�I���B
	__inline void BitMask_On(CollisionType type) {
		if (static_cast<int>(type) >= 26) {
			// �}�X�N�ɐݒ�ł���̂̓f�t�H���g�̃}�X�N��������26bit�܂ŁB
			abort();
		}
		// type���ڂ̃r�b�g��0�ɂ��ă}�X�N���I���ɂ���B
		int mask = 0x00000001;	// 0x00000001�B
		// type�������V�t�g���Ă�������ړI��bit��1�ɂ���B
		mask = mask << static_cast<int>(type);	// 0x00000001�B
		// ���ׂĂ�bit�𔽓]���A�ړI�̃r�b�g�̂�0�A����1�ɂ���B
		mask = ~mask;	// 0xfffffffe�B

		//// �ŏ�ʃr�b�g�͕����r�b�g�̂���0�ɂ���B
		//int work = 0xffffffff;
		//mask = mask & work;	// 0x7ffffffe�B

		if (m_LayerMask != -1) {
			int test = m_LayerMask | mask;	// ��������bit��1�������ꍇ��test�̒l��0xffffffff�ƂȂ�B
			if (test != 0xffffffff) {
				// ��������bit�����Ƃ���0�������B
				return;
			}
		}
		else {
			//// 0xffffffff�ƂȂ��Ă��邽�߁A�����r�b�g�ȊO�𗧂ĂĂ���ҏW�B
			//m_LayerMask = 0x7fffffff;
			// �ݒ莞�ɂ��炩����BulletPhysics���Ő錾����Ă������͍��V�t�g���邽�߁A������ł͋t�̏������s���B
			m_LayerMask = (m_LayerMask >> 1) / btBroadphaseProxy::CharacterFilter;
		}
		// AND���Z�ōŏI�I�ȃ}�X�Nbit���Z�o�B
		SetLayerMask(GetLayerMask() & mask);
		// �}�X�N�ݒ�B
		ConfigCollisionFilterMask();
	}

	// �w�肵���R���W�����^�C�v�̃��C���[�}�X�N�I�t�B
	__inline void BitMask_Off(CollisionType type) {
		if (static_cast<int>(type) >= 26) {
			// �}�X�N�ɐݒ�ł���̂̓f�t�H���g�̃}�X�N��������26bit�܂ŁB
			abort();
		}
		// type���ڂ̃r�b�g��1�ɂ��ă}�X�N���I�t�ɂ���B
		int mask = 0x00000001;	// 0x00000001�B
		// type�������V�t�g�B
		mask = mask << static_cast<int>(type);
		// OR���Z�Ō��̃}�X�N�ɉ��Z�B
		SetLayerMask(GetLayerMask() | mask);
		// �}�X�N�ݒ�B
		ConfigCollisionFilterMask();
	}


	// �O������R���W�����̑�����ݒ�B
	// �����̂�R���W�����I�u�W�F�N�g�ւ̑����ݒ�͕K�����̊֐��ōs���Ă��������B
	// ������btCollisionObject��setUserIndex�֐����Ă΂Ȃ��悤�ɁI
	__inline void ConfigCollisionType(CollisionType type){
		// �R���W�����^�C�v�`���ŕۑ��B
		SetMyType(type);
		// bit�`���ɕϊ����ĕۑ��B
		int bitType = TypeToBitType(type);
		SetMyBitGroup(bitType);
		// BulletPhysics�Ɏ����̑�����ݒ�B
		ConfigCollisionFilterGroup();
	}


	// ���̃R���W���������[���h�ɒǉ�����B
	virtual void AddWorld() = 0;

	// ���̃R���W���������[���h����폜����B
	virtual void RemoveWorld() = 0;


	// �R���W�����I�u�W�F�N�g�{�̂�ԋp�B
	inline btCollisionObject* GetCollision() const {
		return m_collisionObject.get();
	}

	// ���C���[�}�X�N�ԋp�B
	// �߂�l��bit�`���̃}�X�N�B
	inline int GetLayerMask() const{
		return m_LayerMask;
	}

	// �R���W�����{�̂Ɉʒu���ݒ�B
	inline void SetPos(const D3DXVECTOR3& pos) {
		m_collisionObject->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
	}


protected:

	// ���C���[�}�X�N��n���B
	__inline void ConfigCollisionFilterMask() {
		if (m_collisionObject) {
			// �R���W�����I�u�W�F�N�g����������Ă���B
			btBroadphaseProxy* Proxy = m_collisionObject->getBroadphaseHandle();
			if (Proxy) {
				// �u���[�h�t�F�[�Y�n���h������������Ă���B
				if (m_MyType == CollisionType::Map || m_MyType == CollisionType::Player) {
					OutputDebugString("�}�b�v�������̓v���C���[�̃��C���[�}�X�N�ݒ�B");
				}
				Proxy->m_collisionFilterMask = m_LayerMask;
			}
		}
	}

	// �R���W�����̑�����n���B
	__inline void ConfigCollisionFilterGroup() {
		// �R���W�����I�u�W�F�N�g�ɂ��̂܂܂̑�����ݒ�B
		m_collisionObject->setUserIndex(static_cast<int>(m_MyType));

		if (m_collisionObject) {
			// �R���W�����I�u�W�F�N�g����������Ă���B
			btBroadphaseProxy* Proxy = m_collisionObject->getBroadphaseHandle();
			if (Proxy) {
				// �u���[�h�t�F�[�Y�n���h������������Ă���B
				if (m_MyType == CollisionType::Map || m_MyType == CollisionType::Player) {
					OutputDebugString("�}�b�v�������̓v���C���[�̃t�B���^�[�O���[�v�ݒ�B");
				}
				Proxy->m_collisionFilterGroup = m_MyBitGroup;
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
	__inline int TypeToBitType(CollisionType type) {
		if (static_cast<int>(type) >= 26) {
			// �}�X�N�ɐݒ�ł���̂̓f�t�H���g�̃}�X�N��������26bit�܂ŁB
			abort();
		}
		int ret = 0x0001;
		// �^�C�v�̐��������V�t�g�B
		ret = ret << static_cast<int>(type);
		return ret;
	}


	// �R���W�����̑����𑮐��`���ŕۑ��B
	inline void SetMyType(CollisionType type) {
		m_MyType = type;
	}
	// �R���W�����̑�����bit�`���ŕۑ��B
	__inline void SetMyBitGroup(int bit) {
		// �����I�ɂ���Ă���l�͎g�p�ł��Ȃ����߁A���̕����V�t�g�ϊ����Đݒ�B
		if (bit != btBroadphaseProxy::CollisionFilterGroups::AllFilter && bit != 0x80000000/*0*/) {
			// BulletPhysics�Ŋ��Ɏg�p����Ă���񋓑̂��㏑�����邽�߁A���̗񋓎q�̍ō��l��2�ȏ�������Ĉ����bit�𗧂Ă�B
			bit = (bit << 1) * btBroadphaseProxy::CharacterFilter;
		}
		m_MyBitGroup = bit;
	}
	// ���C���[�}�X�N�Z�b�g�B
	// ������bit�`���̃}�X�N�B
	__inline void SetLayerMask(int mask) {
		if (mask != btBroadphaseProxy::CollisionFilterGroups::AllFilter && mask != 0x80000000/*0*/) {
			// BulletPhysics�Ŋ��Ɏg�p����Ă���񋓑̂��㏑�����邽�߁A���̗񋓎q�̍ō��l��2�ȏ�������Ĉ����bit�𗧂Ă�B
			mask = (mask << 1) * btBroadphaseProxy::CharacterFilter;
		}
		//// �����r�b�g�𗧂ĂĂ������B
		//mask = mask | 0x8000;
		m_LayerMask = mask;
	}
protected:
	//��������bulletPhysics�̍��̂��g�p���邽�߂ɕK�v�ȕϐ��B
	btVector3 m_OriginOffset;		// �I�u�W�F�N�g�̒��S�ƍ��̂̒��S�̍����B
	unique_ptr<btCollisionShape>	m_collisionShape;	//�R���W�����̌`��B
	unique_ptr<btCollisionObject>		m_collisionObject;		//���́B
	CollisionType m_MyType = CollisionType::None;
	// ���̃R���W�����̑���(bit)�B
	// ���R���W���������[���h�ɓo�^�����Ƃ��ɂ͂��߂Ēʒm����B
	int m_MyBitGroup = 0x0000;
	// ������𖳎����鑮����bit���Ŋi�[(�Փ˂��鑮����bit��1�𗧂Ă�)�B
	// ���R���W���������[���h�ɓo�^�����Ƃ��ɂ͂��߂Ēʒm����B
	int m_LayerMask = btBroadphaseProxy::CollisionFilterGroups::AllFilter;
	CBulletPhysics* m_pBulletPhysics;	// �������[���h�ƃR���W�������[���h�����N���X�B
};

