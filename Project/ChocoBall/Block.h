#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "Rigidbody.h"
#include "islntersect.h"
#include "ObjectManager.h"
#include "C3DObjectRender_S.h"


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
		m_RenderingState = RENDER_STATE::_3D_Simple;
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
	bool IsDead()
	{
		return m_isDead;
	}
private:
	//�q����ݒ�B
	void SetChild(CBlock* child)
	{
		m_child = child;
	}
private:
	CIsIntersect	m_IsIntersect;
	enum EnState{
		enState_Normal,	//�ʏ��ԁB
		enState_Broken,	//��ꂽ�B
		enState_Fall,	//�������B
	};
	CBlock*			m_parent;		//�e�u���b�N
	CBlock*			m_child;		//�q��
	D3DXVECTOR3		m_moveSpeed;	//�������x
	float			m_radius;
	//��������bulletPhysics�̍��̂��g�p���邽�߂ɕK�v�ȕϐ��B
	btRigidBody*		m_rigidBody;		//���́B
	btCollisionShape*	m_collisionShape;	//�R���W�����̌`��B
	btDefaultMotionState* m_myMotionState;
	bool m_life;
	EnState				m_eState;	//��ԁB
	float				m_fallPosY;	//�����ʒu�B
	bool m_isDead;
	D3DXHANDLE m_hShaderTecnique;	//�V�F�[�_�[�e�N�j�b�N�̃n���h���B
};