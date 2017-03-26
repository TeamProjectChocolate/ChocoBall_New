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

	// チョコ壁を飛ばす。
	void Throw(const D3DXVECTOR3& dir, float Power,float Time);

	//親が死んだときに呼ばれる処理。
	void OnDestroyParent();

	//親を設定。
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

	// 死の宣告を与える関数。
	// 引数:	削除までの猶予時間。
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
	// このチョコ壁が飛ばされているかのフラグを設定。
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
	// プレイヤーとの衝突判定。
	void CollisionPlayer();
	// 死亡までカウント。
	void DeathCount();
	//子供を設定。
	void SetChild(CBlock* child)
	{
		m_child = child;
	}
	// 子との縁を切る。
	inline void RemoveChild() {
		m_child = nullptr;
	}
	// 親との縁を切る。
	inline void RemoveParent() {
		m_parent = nullptr;
	}
private:
	enum EnState {
		enState_Normal,	//通常状態。
		enState_Broken,	//壊れた。
		enState_Fall,	//落下中。
		enState_Crumble,	// 自壊。
	};
	CBlock*			m_parent;		//親ブロック
	CBlock*			m_child;		//子供
	D3DXVECTOR3		m_moveSpeed;	//落下速度
	float			m_radius;
	bool m_life;
	EnState				m_eState;	//状態。
	float				m_fallPosY;	//落下位置。
	bool m_isDead;
	D3DXHANDLE m_hShaderTecnique;	//シェーダーテクニックのハンドル。
	bool m_IsThrow = false;

	CBulletPhysics* m_pBulletPhysics = nullptr;

	CPlayer* m_Player;
	float m_ThrowTime = 0.0f;
	float m_ThrowCounter = 0.0f;
	float m_DeathTime = 0.0f;
	float m_DeathCounter = 0.0f;
};