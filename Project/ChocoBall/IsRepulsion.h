#pragma once
#include "BulletPhysics.h"
#include "CollisionType.h"

// ���͂̍��̂ɂ��˗͂��I�u�W�F�N�g�ɔ��f����N���X�B
class CIsRepulsion
{
public:
	CIsRepulsion();
	~CIsRepulsion();
	void Initialize(CCollisionInterface* collision);
	// �˗͂����I�u�W�F�N�g�ɐ˗͂̍��v�e���l��ԋp����֐��B
	// �����F	�I�u�W�F�N�g�̈ړ����x(�֐������Ő˗͂����Z)�B
	void Repulsion(D3DXVECTOR3* moveSpeed);
private:
	CCollisionInterface* m_pCollision = nullptr;
};

