#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "Rigidbody.h"
#include "islntersect.h"
#include "ObjectManager.h"
#include "C3DObjectRender_S.h"

#include "ParticleEmitter.h"

class CBlock : public CGameObject{
public:
	CBlock(){ 
		SetLife(true); 
		m_parent = NULL;
		m_child = NULL;
		m_isDead = false;
		m_eState = enState_Normal;
		m_fallPosY = 0.0f;
	}
	~CBlock() {
		//this->OnDestroy();
	}
	void OnDestroy()override;
	void Initialize(D3DXVECTOR3 pos, D3DXQUATERNION rot);
	void Update();
	void Draw();

	void BeginDraw();
	void EndDraw();

	void SetUpTechnique()override{
#ifdef NOT_EM
		m_pRender->SetUpTechnique("Boneless_Tex_Lim");
#else
		m_pRender->SetUpTechnique("Boneless_Tex_Fresnel");
#endif
	}
	void EM_SetUpTechnique()override{
		m_pEMSamplingRender->SetUpTechnique("Boneless_Tex_Lim");
	}

	void SetRenderState()override{
		m_RenderingState = RENDER::TYPE::_3D_Simple;
	}

	void Setradius(float radius)
	{
		m_radius = radius;
	}
	void Build(const D3DXVECTOR3& size, const D3DXVECTOR3& pos);
	void SetLife(bool life){
		m_life = life;
	}
	bool GetLife(){
		return m_life;
	}

	// �`���R�ǂ��΂��B
	void Throw(const D3DXVECTOR3& dir, float Power,float Time);

	//�e�����񂾂Ƃ��ɌĂ΂�鏈���B
	void OnDestroyParent();

	//�e��ݒ�B
	void SetParent(CBlock* par)
	{
		if (par){
			m_parent = par;
			par->SetChild(this);
		}
		else{
			m_parent = NULL;
		}
	}

	// ���̐鍐��^����֐��B
	// ����:	�폜�܂ł̗P�\���ԁB
	inline void SentenceOfDeath(float DeathTime) {
		m_DeathTime = DeathTime;
		m_DeathCounter = 0.0f;
	}

	bool IsDead()
	{
		return m_isDead;
	}
	void SetCollisionType(Collision::Type type) {
		m_CollisionObject->SetCollisionType(type);
	}
	// ���̃`���R�ǂ���΂���Ă��邩�̃t���O��ݒ�B
	inline void SetIsThrow(bool flg) {
		m_IsThrow = flg;
	}
	inline void SetPlayer(CPlayer* Obj) {
		m_Player = Obj;
	}

	inline void SetBulletPhisics(CBulletPhysics* BP){
		m_pBulletPhysics = BP;
	}
private:
	// �v���C���[�Ƃ̏Փ˔���B
	void CollisionPlayer();
	// ���S�܂ŃJ�E���g�B
	void DeathCount();
	//�q����ݒ�B
	void SetChild(CBlock* child)
	{
		m_child = child;
	}
	// �q�Ƃ̉���؂�B
	inline void RemoveChild() {
		m_child = nullptr;
	}
	// �e�Ƃ̉���؂�B
	inline void RemoveParent() {
		m_parent = nullptr;
	}
private:
	enum EnState {
		enState_Normal,	//�ʏ��ԁB
		enState_Broken,	//��ꂽ�B
		enState_Fall,	//�������B
		enState_Crumble,	// ����B
	};
	CBlock*			m_parent;		//�e�u���b�N
	CBlock*			m_child;		//�q��
	D3DXVECTOR3		m_moveSpeed;	//�������x
	float			m_radius;
	bool m_life;
	EnState				m_eState;	//��ԁB
	float				m_fallPosY;	//�����ʒu�B
	bool m_isDead;
	D3DXHANDLE m_hShaderTecnique;	//�V�F�[�_�[�e�N�j�b�N�̃n���h���B
	bool m_IsThrow = false;

	CBulletPhysics* m_pBulletPhysics = nullptr;

	CPlayer* m_Player;
	float m_ThrowTime = 0.0f;
	float m_ThrowCounter = 0.0f;
	float m_DeathTime = 0.0f;
	float m_DeathCounter = 0.0f;
};