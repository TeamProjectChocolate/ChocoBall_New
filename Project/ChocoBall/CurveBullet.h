#pragma once
#include "Bullet.h"

// �Ȑ��O������e�̃N���X�B
class CCurveBullet :
	public Bullet
{
public:
	CCurveBullet();
	~CCurveBullet();

	// �e�̏������B
	// �����F	���ˈʒu�B
	//			�ڕW�n�_(���̕����ɏ��X�ɋO���C������)�B
	//			�ŏ��ɐݒ肵�����e�̔�ԕ����B
	//			1�t���[���ŉ�]�ł���ʁB
	void Initialize(const D3DXVECTOR3& ShotPos,D3DXVECTOR3* TargetPos, const D3DXVECTOR3& InitDir, float MaxAngle);
	void Update()override;

	inline void SetTargetPoint(D3DXVECTOR3* pos) {
		m_TargetPoint = pos;
	}
	// �����͓x�B
	inline void SetMaxAngle(float angle) {
		m_MaxAngle = angle;
	}
	// ���݂̌�����ݒ�B
	// �����͂��̊֐����Ő��K�������B
	inline void SetDir(const D3DXVECTOR3& dir)override{
		D3DXVec3Normalize(&m_NowDir,&dir);
	}
private:
	//enum class BulletState{Curve = 0};
private:
	D3DXVECTOR3* m_TargetPoint = nullptr;	// �e���ŏI�I�ɖ������������ʒu�B
	D3DXVECTOR3 m_NowDir;	// ���݂̒e�̌����B
	float m_MaxAngle;	// 1�t���[���Œe����]�ł�����E��(�x)�B
	D3DXQUATERNION m_RotationAngle;	// ���ۂ̉�](���v�l)�B
	D3DXVECTOR3 m_ShotToTarget;	// ���˒n�_����ڕW�܂ł̃x�N�g���B
};

