#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "ObjectManager.h"
#include "Block.h"
#include "LevelBuilder.h"
#include "GameObject.h"

#define BUILD_H 5		//生成される壁の数(縦)
#define BUILD_W 6		//生成される壁の数(横)

#define BLOCK_H 1.0f		//ブロックのたかさ
#define BLOCK_W 1.0f		//ブロックの幅


class CBuildBlock : public CGameObject{
public:
	CBuildBlock();
	~CBuildBlock();
	void Initialize()override
	{
	}
	void Update()override;
	void Draw()override;
	void DrawShadow(CCamera*)override;
	void Draw_EM(CCamera*)override;

	void Initialize(D3DVECTOR pos, D3DXQUATERNION rot);

	void SetRenderState()override{
		m_RenderingState = RENDER::TYPE::Instancing;
	}
	void SetShadowRenderState()override{
		m_ShadowRenderingState = RENDER::TYPE::_3D_ShadowSample_I;
	}
	void EM_SetRenderState()override{
			m_EMRenderingState = RENDER::TYPE::EM_Sampling_I;
	}

	void ActivateShadowRender()override;

	void SetUpTechnique()override{
#ifdef NOT_EM
		m_pRender->SetUpTechnique("IBoneless_Tex_Lim");
#else
		m_pRender->SetUpTechnique("IBoneless_Tex_Fresnel");
#endif
	}
	void EM_SetUpTechnique()override{
		m_pEMSamplingRender->SetUpTechnique("IBoneless_Tex_Lim");
	}

	CBlock* GetBlocks(int x,int y){
		return &m_blocks[y][x];
	}
	int GetNum_Y(){
		return BUILD_H;
	}
	int GetNum_X(){
		return BUILD_W;
	}
	float GetBlockHeight(){
		return BLOCK_H;
	}
	float GetBlockWidth(){
		return BLOCK_W;
	}
	void SetPintoWorld(const D3DXMATRIX& mat)override{
#ifdef NOT_DOF
		for (short row = 0; row < BUILD_H; row++) {
			for (short col = 0; col < BUILD_W; col++) {
				m_blocks[row][col].SetPintoWorld(mat);
			}
		}
#else
		m_pModel->SetPintoWorld(mat);
#endif
	}
	void SetPintoPos(const D3DXVECTOR3& pos)override{
#ifdef NOT_DOF
		for (short row = 0; row < BUILD_H; row++) {
			for (short col = 0; col < BUILD_W; col++) {
				m_blocks[row][col].SetPintoPos(pos);
			}
		}
#else
		m_pModel->SetPintoPos(pos);
#endif
	}

	// チョコ壁を飛ばす関数。
	void ThrowBlock(const D3DXVECTOR3& dir, float Power,float Time);

	static int GetBBManagerNum() {
		return m_BBManagerNum;
	}
private:
	CBlock	m_blocks[BUILD_H][BUILD_W];	//壁の配列
	static int m_BBManagerNum;
	D3DXVECTOR3 m_Direction;	// チョコ壁を飛ばす方向。
	float m_ThrowPower;	// チョコ壁が飛ぶ強さ。
	bool m_IsThrow;	// このチョコ壁は飛ぶか。
};