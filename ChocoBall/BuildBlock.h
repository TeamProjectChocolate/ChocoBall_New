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
	void DrawDepth(const D3DXVECTOR2&,const D3DXVECTOR3&,const D3DXMATRIX&)override;
	void Initialize(D3DVECTOR pos, D3DXQUATERNION rot);
	void SetRenderState()override{
		if (m_IsInstancing){
			m_RenderingState = RENDER_STATE::Instancing;
		}
		else{
			return;
		}
	}
	void SetDepthRenderState()override{
		m_DepthRenderingState = RENDER_STATE::Instancing_Depth;
	}
	void SetUpTechnique()override{
		m_pRender->SetUpTechnique("NonAnimationInstancing_Fresnel");
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

private:
	CBlock	m_blocks[BUILD_H][BUILD_W];	//壁の配列
	static int m_BBManagerNum;
};