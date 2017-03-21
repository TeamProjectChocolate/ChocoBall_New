#pragma once
#include "CollisionInterface.h"

class CCollisionObject :
	public CCollisionInterface
{
public:
	CCollisionObject();
	~CCollisionObject()override;
	void Update(D3DXVECTOR3* pos, D3DXQUATERNION* rot)override;

	// ���̃R���W�����I�u�W�F�N�g���R���W�������[���h�ɒǉ�����B
	void AddWorld()override;
	// ���̃R���W�����I�u�W�F�N�g���R���W�������[���h����폜����B
	void RemoveWorld()override;
private:
	// �p�����[�^���Q�Ƃ��č��̐����B
	// �����F�I�u�W�F�N�g�̃g�����X�t�H�[�����B
	//		 ����(isKinematic��true�Ȃ�Ύ����I��0.0f�ɂȂ�)�B
	//		 �g�p����Ă��܂���B
	void Build(
		const btTransform& transform,
		float mass,
		bool flg)override;
};

