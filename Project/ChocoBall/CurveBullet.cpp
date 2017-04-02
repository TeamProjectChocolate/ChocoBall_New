#include "stdafx.h"
#include "CurveBullet.h"


CCurveBullet::CCurveBullet()
{
}


CCurveBullet::~CCurveBullet()
{
}

void CCurveBullet::Initialize(const D3DXVECTOR3& ShotPos, D3DXVECTOR3* TargetPos, const D3DXVECTOR3& InitDir, float MaxAngle) {
	Bullet::Initialize();
	SetTargetPoint(TargetPos);
	SetDir(InitDir);
	SetMaxAngle(MaxAngle);
	//// �����ʒu����ڕW�n�_�̃x�N�g�����Z�o�B
	//m_ShotToTarget = TargetPos - ShotPos;
	SetStartPos(ShotPos);
	m_RotationAngle = Quaternion::Identity;
}

void CCurveBullet::Update() {
	//// �e���ڕW�n�_�������ɂ��邩����B
	//// �����ʒu���猻�݂̈ʒu�ւ̃x�N�g�����Z�o�B
	//D3DXVECTOR3 ShotToNow = m_transform.position - m_StartPos;
	//// �����ʒu����ڕW�n�_�܂ł̌����x�N�g���Z�o�B
	//D3DXVECTOR3 ShotToTargetDirection;
	//D3DXVec3Normalize(&ShotToTargetDirection,&m_ShotToTarget);

	//// �����ʒu����^�[�Q�b�g�ւ̐���ł̒������Z�o�B
	//float Length = D3DXVec3Dot(&ShotToTargetDirection, &ShotToNow);
	//if (Length <= 0.001f) {
	//
	//}


	D3DXVECTOR3 TargetDir;
	// ���̈ʒu����ڕW�ʒu�܂ł̌����x�N�g�����Z�o�B
	D3DXVec3Normalize(&TargetDir,&(*m_TargetPoint - m_transform.position));

	// �ڕW�����x�N�g���ƌ��݂̌����x�N�g���̊p�x���Z�o(���W�A��)�B
	float rad = acosf(D3DXVec3Dot(&m_NowDir, &TargetDir));	// ��������������]�ʁB
	float unsignedRad = fabsf(rad);	// �����������Ȃ���]�ʁB
	float dir = rad / unsignedRad;	// ��]�����B
	float maxRad = D3DXToRadian(m_MaxAngle);	// �O������󂯎�����l�����W�A���ɕϊ��B
	if (maxRad >= unsignedRad) {
		// 1�t���[���̉�]�p�x�̂ق����傫���B
		maxRad = rad;
	}
	else {
		// ��]������������B
		maxRad *= dir;
	}

	D3DXVECTOR3 Axis;
	D3DXVec3Cross(&Axis, &m_NowDir, &TargetDir);
	D3DXVec3Normalize(&Axis, &Axis);

	// �p�x��p���Ēe�̌����x�N�g������]�B
	D3DXQUATERNION work;
	work = Quaternion::Identity;
	D3DXQuaternionRotationAxis(&work, &Axis, maxRad);

	// �N�H�[�^�j�I�������]�s��쐬�B
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixRotationQuaternion(&mat,&work);

	// ��]�s��Ō����x�N�g������]�B
	D3DXVec3TransformNormal(&m_NowDir, &m_NowDir, &mat);

	// ���N���X�Œe�̈ʒu�����X�V�B
	Bullet::SetDir(m_NowDir);
	Bullet::Update();
}