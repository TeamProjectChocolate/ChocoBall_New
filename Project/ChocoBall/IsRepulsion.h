#pragma once
#include "BulletPhysics.h"
#include "CollisionType.h"

// ���͂̍��̂ɂ��˗͂��I�u�W�F�N�g�ɔ��f����N���X�B
class CIsRepulsion
{
public:
	CIsRepulsion();
	~CIsRepulsion();
	void Initialize(btCollisionObject* collision);
	// �˗͂����I�u�W�F�N�g�ɐ˗͂̍��v�e���l��ԋp����֐��B
	// �����F �I�u�W�F�N�g�̈ʒu���B
	//		  �I�u�W�F�N�g�̈ړ����x(�֐������Ő˗͂����Z)�B
	void IsRepulsion(const D3DXVECTOR3& pos,D3DXVECTOR3& moveSpeed);
private:
	btCollisionObject* m_pCollision = nullptr;
};

